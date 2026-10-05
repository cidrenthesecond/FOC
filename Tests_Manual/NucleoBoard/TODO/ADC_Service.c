#include "stm32h533xx.h"
#include "stm32h5xx_ll_adc.h"
#include "ADC_Service.h"
#include "Moving_Avarage_Filter.h"

#include "Logger.h"
#include "stdio.h"

#define ADC_FULL_SCALE            4095
#define ADC_VREF_MV               3300
#define BUS_VOLTAGE_DIVIDER_RATIO 126  
#define ADC_OFFSET_SAMPLES_COUNT  50U
#define FILTER_KERNEL             3

static const uint32_t adc_channel_map[5] =
{
    [PHASE_A_CHANNEL]     = LL_ADC_CHANNEL_3,
    [PHASE_B_CHANNEL]     = LL_ADC_CHANNEL_4,
    [PHASE_C_CHANNEL]     = LL_ADC_CHANNEL_7,
    [BUS_VOLTAGE_CHANNEL] = LL_ADC_CHANNEL_1,
    [NTC_VOLTAGE_CHANNEL] = LL_ADC_CHANNEL_0
};

static uint16_t phase_a_offset_adc;
static uint16_t phase_b_offset_adc;
static uint16_t phase_c_offset_adc;

static MovingAvarage NTC_Filter;
static MovingAvarage DcBus_Filter;
static MovingAvarage Phase_A_Filter;
static MovingAvarage Phase_B_Filter;
static MovingAvarage Phase_C_Filter;

static void (*OverCurrentInterruptCallback)(void);

typedef union {
    uint8_t raw;
    struct {
        uint8_t ready                   : 1; // Bit 0
        uint8_t error                   : 1; // Bit 1
        uint8_t busy                    : 1; // Bit 2
        uint8_t phase_offset_configured : 1; // Bit 3
        uint8_t reserved                : 4; // Bity 4-7
    } bits;
} StatusRegister_t;

static volatile StatusRegister_t status = {0};

static void ADC_CalibratePhaseOffsets(void);
static void ADC_PrepareForPhaseOffsetMeasurement(void);
static void ADC_GatherOffsetData(uint16_t * a_data,uint16_t * b_data, uint16_t * c_data);
static void ADC_CalculatePhaseOffsets(uint16_t *a_data, uint16_t *b_data, uint16_t * c_data);
static void ADC_Calibrate();

void ADC_Init()
{
  NTC_Filter     = MovingAvarage_Init(FILTER_KERNEL);
  DcBus_Filter   = MovingAvarage_Init(FILTER_KERNEL);
  Phase_A_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_B_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_C_Filter = MovingAvarage_Init(FILTER_KERNEL);
  OverCurrentInterruptCallback = NULL;

  ADC_Calibrate();
  ADC_CalibratePhaseOffsets();
  ADC_Set_OCP_Threshold(6000);

  LL_ADC_Enable(ADC1);
  LL_ADC_INJ_StartConversion(ADC1);
  LL_ADC_EnableIT_AWD2(ADC1);
}

void ADC_PrepareForControlLoop()
{
  MovingAvarage_Reset(DcBus_Filter);
  MovingAvarage_Reset(Phase_A_Filter);
  MovingAvarage_Reset(Phase_B_Filter);
  MovingAvarage_Reset(Phase_C_Filter);
}

Analog_Feedback_t ADC_GetAnalogFeedback()
{
  Analog_Feedback_t result;

  uint16_t phase_a_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, adc_channel_map[PHASE_A_CHANNEL]);
  uint16_t phase_b_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, adc_channel_map[PHASE_B_CHANNEL]);
  uint16_t phase_c_measurement = LL_ADC_INJ_ReadConversionData12(ADC1, adc_channel_map[PHASE_C_CHANNEL]);
  uint16_t dcBus_measurement   = LL_ADC_INJ_ReadConversionData12(ADC1, adc_channel_map[BUS_VOLTAGE_CHANNEL]);

  phase_a_measurement = MovingAvarage_Filter(Phase_A_Filter, phase_a_measurement);
  phase_b_measurement = MovingAvarage_Filter(Phase_B_Filter, phase_b_measurement);
  phase_c_measurement = MovingAvarage_Filter(Phase_C_Filter, phase_c_measurement);
  dcBus_measurement   = MovingAvarage_Filter(DcBus_Filter, dcBus_measurement);

  result.Currents = ADC_CalculatePhaseCurrents(phase_a_measurement, phase_b_measurement, phase_c_measurement);
  result.DcBus    = ADC_CalculateDcLinkVoltage(dcBus_measurement);

  return result;
}

uint16_t ADC_ReadSingleChannelRaw(ADC_Channel_t channel)
{
  uint32_t ll_channel = adc_channel_map[channel];

  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, ll_channel);
  LL_ADC_REG_StartConversion(ADC1);

  while (!LL_ADC_IsActiveFlag_EOC(ADC1))
      ;

  LL_ADC_ClearFlag_EOC(ADC1);

  return LL_ADC_REG_ReadConversionData12(ADC1);
}


//______________DC Link_____________

uint32_t ADC_CalculateDcLinkVoltage(uint16_t adcMeasurement)
{
	return BUS_VOLTAGE_DIVIDER_RATIO * ADC_VREF_MV * adcMeasurement / ADC_FULL_SCALE;
}
uint32_t ADC_GetDcLinkVoltage()
{
  return ADC_CalculateDcLinkVoltage(ADC_ReadSingleChannelRaw(BUS_VOLTAGE_CHANNEL));
}

//_____________NTC______________

//TO DO
uint16_t ADC_GetNtcVoltage()
{
  uint16_t measurement = ADC_ReadSingleChannelRaw(NTC_VOLTAGE_CHANNEL);
  uint16_t NTC_Measurement      = MovingAvarage_Filter(NTC_Filter, measurement);
  return ADC_CalculateNtcVoltage(NTC_Measurement);
}

uint16_t ADC_CalculateNtcVoltage(uint16_t adcMeasurement)
{
  return adcMeasurement*ADC_VREF_MV / ADC_FULL_SCALE;
}

//____________Phase Currents______________



Phase_Currents_t ADC_CalculatePhaseCurrents(uint16_t ADC_Phase_A, uint16_t ADC_Phase_B, uint16_t ADC_Phase_C)
{
  Phase_Currents_t result;
  result.Current_A = ((int32_t)ADC_Phase_A - (int32_t)phase_a_offset_adc) *3128/1000;
  result.Current_B = ((int32_t)ADC_Phase_B - (int32_t)phase_b_offset_adc) *3128/1000;
  result.Current_C = ((int32_t)ADC_Phase_C - (int32_t)phase_c_offset_adc) *3128/1000;
  return result;
}


static void ADC_Calibrate()
{
  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);

  while(LL_ADC_IsCalibrationOnGoing(ADC1))
	  ;
}

//_______________OCP__________________

void ADC_Set_OCP_Threshold(uint16_t thresholdCurrent_ma)
{
  if(status.bits.phase_offset_configured == 0)
    return;

  uint16_t thresholdOffset = thresholdCurrent_ma*1000/3128;
  uint16_t midpointAvgg    = (phase_a_offset_adc+phase_b_offset_adc+phase_c_offset_adc)/3;
  uint16_t thresholdHigh   = midpointAvgg + thresholdOffset;
  uint16_t thresholdLow    = midpointAvgg - thresholdOffset;

  LL_ADC_ConfigAnalogWDThresholds(ADC1, LL_ADC_AWD2, (thresholdHigh>>4), (thresholdLow>>4));
}

void ADC_Set_OCP_ISR(void (*isr)(void))
{
  if(isr == NULL)
    return;

  OverCurrentInterruptCallback = isr;
}

//________________Helper functions________________

static void ADC_CalibratePhaseOffsets(void)
{
  ADC_PrepareForPhaseOffsetMeasurement();

  uint16_t a_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t b_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t c_data[ADC_OFFSET_SAMPLES_COUNT];

  ADC_GatherOffsetData(a_data,b_data,c_data);
  ADC_CalculatePhaseOffsets(a_data, b_data, c_data);
  status.bits.phase_offset_configured = 1;

  char buffer[64] = {0};
  sprintf(buffer, "ADC: offsets: A %u, B %u, C %u",
          phase_a_offset_adc,phase_b_offset_adc,phase_c_offset_adc);
  LOG(buffer);

  //return state to
  LL_ADC_Disable(ADC1);
  LL_ADC_INJ_SetTriggerSource(ADC1, LL_ADC_INJ_TRIG_EXT_TIM1_TRGO);
}

static void ADC_PrepareForPhaseOffsetMeasurement()
{
  LL_ADC_INJ_SetTriggerSource(ADC1, LL_ADC_INJ_TRIG_SOFTWARE);
  LL_ADC_Enable(ADC1);
  while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
      ; // Czekamy aż przetwornik będzie gotowy

  LL_ADC_ClearFlag_JEOS(ADC1);
  LL_ADC_INJ_StartConversion(ADC1);
  while (!LL_ADC_IsActiveFlag_JEOS(ADC1))
    ;
  LL_ADC_ClearFlag_JEOS(ADC1);
}

static void ADC_GatherOffsetData(uint16_t * a_data,uint16_t * b_data, uint16_t * c_data)
{
  for(uint8_t i = 0; i < ADC_OFFSET_SAMPLES_COUNT; i++)
  {
    LL_ADC_INJ_StartConversion(ADC1);

    while(!LL_ADC_IsActiveFlag_JEOS(ADC1))
      ;

    a_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_2);
    b_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
    c_data[i] = LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_4);

    LL_ADC_ClearFlag_JEOS(ADC1);
  }
}

static void ADC_CalculatePhaseOffsets(uint16_t *a_data, uint16_t *b_data, uint16_t * c_data)
{
  uint32_t a_sum = 0, b_sum = 0, c_sum = 0;

  for(uint8_t i = 0 ; i < ADC_OFFSET_SAMPLES_COUNT; i++)
  {
    a_sum += a_data[i];
    b_sum += b_data[i];
    c_sum += c_data[i];
  }

  phase_a_offset_adc = a_sum /ADC_OFFSET_SAMPLES_COUNT;
  phase_b_offset_adc = b_sum /ADC_OFFSET_SAMPLES_COUNT;
  phase_c_offset_adc = c_sum /ADC_OFFSET_SAMPLES_COUNT;
}

void PrintFloat(float x)
{
  int decimal    = (int)x;
  int fractional = (int)((x - decimal)*100);
  char buffer[30];
  sprintf(buffer, "B: %d.%02d", decimal,fractional);
  LOG(buffer);
}

/**
  * @brief This function handles ADC1 global interrupt.
  */
void ADC1_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_IRQn 0 */
  if(LL_ADC_IsActiveFlag_AWD2(ADC1))
  {
    OverCurrentInterruptCallback();
    LL_ADC_ClearFlag_AWD2(ADC1);
  }
  /* USER CODE END ADC1_IRQn 0 */
  /* USER CODE BEGIN ADC1_IRQn 1 */

  /* USER CODE END ADC1_IRQn 1 */
}
