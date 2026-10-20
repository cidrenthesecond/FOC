#include "stm32h533xx.h"
#include "stm32h5xx_ll_adc.h"
#include "ADC_Service.h"
#include "Moving_Avarage_Filter.h"

#include "Logger.h"
#include "stdio.h"

//-------------------BSP----------------------------
static const uint32_t adc_channel_map[5] =
{
  [PHASE_A_CHANNEL]     = LL_ADC_CHANNEL_3,
  [PHASE_B_CHANNEL]     = LL_ADC_CHANNEL_4,
  [PHASE_C_CHANNEL]     = LL_ADC_CHANNEL_7,
  [BUS_VOLTAGE_CHANNEL] = LL_ADC_CHANNEL_1,
  [NTC_VOLTAGE_CHANNEL] = LL_ADC_CHANNEL_0
};

#define ADC_USED ADC1
//--------------------------------------------------

#define ADC_FULL_SCALE            4095
#define ADC_VREF_MV               3300
#define BUS_VOLTAGE_DIVIDER_RATIO 126
#define ADC_OFFSET_SAMPLES_COUNT  50U
#define FILTER_KERNEL             3
#define ADC_OFFSET_TOP_MARGAIN    2100u
#define ADC_OFFSET_BOTTOM_MARGAIN 1900u

#define FLAG_ADC_OFFSETS_CALIBRATED (1U << 0)
#define FLAG_ADC_INITIALISED        (1U << 1)

typedef struct {
  uint16_t a;
  uint16_t b;
  uint16_t c;
} Phase_Offsets_t;

static Phase_Offsets_t phase_offsets;

static MovingAvarage NTC_Filter;
static MovingAvarage DcBus_Filter;
static MovingAvarage Phase_A_Filter;
static MovingAvarage Phase_B_Filter;
static MovingAvarage Phase_C_Filter;

static void (*OverCurrentInterruptCallback)(void);

static ADC_Status_t ADC_CalibratePhaseOffsets(void);
static void ADC_GatherOffsetData(uint16_t * a_data,uint16_t * b_data, uint16_t * c_data);
static Phase_Offsets_t ADC_CalculatePhaseOffsets(uint16_t *a_data, uint16_t *b_data, uint16_t * c_data);
static void ADC_Calibrate();
static ADC_Status_t ADC_ValidatePhaseOffsets(Phase_Offsets_t offsets);

static uint8_t ADC_Status = 0;

ADC_Status_t ADC_Init()
{
  NTC_Filter     = MovingAvarage_Init(FILTER_KERNEL);
  DcBus_Filter   = MovingAvarage_Init(FILTER_KERNEL);
  Phase_A_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_B_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_C_Filter = MovingAvarage_Init(FILTER_KERNEL);
  OverCurrentInterruptCallback = NULL;

  ADC_Calibrate();

  ADC_CalibratePhaseOffsets();

  ADC_Set_OCP_Threshold(6000);  // In need of config

  LL_ADC_Enable(ADC_USED);
  LL_ADC_INJ_StartConversion(ADC_USED);
  LL_ADC_EnableIT_AWD2(ADC_USED);

  return ADC_SUCCESS;
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

  uint16_t phase_a_measurement = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_A_CHANNEL]);
  uint16_t phase_b_measurement = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_B_CHANNEL]);
  uint16_t phase_c_measurement = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_C_CHANNEL]);
  uint16_t dcBus_measurement   = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[BUS_VOLTAGE_CHANNEL]);

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

  LL_ADC_REG_SetSequencerRanks(ADC_USED, LL_ADC_REG_RANK_1, ll_channel);
  LL_ADC_REG_StartConversion(ADC_USED);

  while (!LL_ADC_IsActiveFlag_EOC(ADC_USED))
    ;

  LL_ADC_ClearFlag_EOC(ADC_USED);

  return LL_ADC_REG_ReadConversionData12(ADC_USED);
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
  result.Current_A = ((int32_t)ADC_Phase_A - (int32_t)phase_offsets.a) *3128/1000;
  result.Current_B = ((int32_t)ADC_Phase_B - (int32_t)phase_offsets.b) *3128/1000;
  result.Current_C = ((int32_t)ADC_Phase_C - (int32_t)phase_offsets.c) *3128/1000;
  return result;
}


static void ADC_Calibrate()
{
  LL_ADC_StartCalibration(ADC_USED, LL_ADC_SINGLE_ENDED);

  while(LL_ADC_IsCalibrationOnGoing(ADC_USED))
	  ;
}

//________________Helper functions________________

static ADC_Status_t ADC_CalibratePhaseOffsets(void)
{
  uint16_t a_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t b_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t c_data[ADC_OFFSET_SAMPLES_COUNT];

  LL_ADC_Enable(ADC_USED);
  ADC_GatherOffsetData(a_data,b_data,c_data);
  Phase_Offsets_t offsets = ADC_CalculatePhaseOffsets(a_data, b_data, c_data);
  
  if(ADC_ValidatePhaseOffsets(offsets) == ADC_FAIL)
    return ADC_FAIL;

  phase_offsets.a = offsets.a;
  phase_offsets.b = offsets.b;
  phase_offsets.c = offsets.c;

  char buffer[64] = {0};
  sprintf(buffer, "ADC: offsets: A %u, B %u, C %u",
          phase_offsets.a,
          phase_offsets.b,
          phase_offsets.c);
  LOG(buffer);

  //return state to
  LL_ADC_Disable(ADC_USED);
  LL_ADC_INJ_SetTriggerSource(ADC_USED, LL_ADC_INJ_TRIG_EXT_TIM1_TRGO);

  return ADC_SUCCESS;
}

static void ADC_GatherOffsetData(uint16_t * a_data,uint16_t * b_data, uint16_t * c_data)
{
  for(uint8_t i = 0; i < ADC_OFFSET_SAMPLES_COUNT; i++)
  {
    a_data[i] = ADC_ReadSingleChannelRaw(PHASE_A_CHANNEL);
    b_data[i] = ADC_ReadSingleChannelRaw(PHASE_B_CHANNEL);
    c_data[i] = ADC_ReadSingleChannelRaw(PHASE_C_CHANNEL);
  }
}

static Phase_Offsets_t ADC_CalculatePhaseOffsets(uint16_t *a_data, uint16_t *b_data, uint16_t * c_data)
{
  uint32_t a_sum = 0, b_sum = 0, c_sum = 0;

  for(uint8_t i = 0 ; i < ADC_OFFSET_SAMPLES_COUNT; i++)
  {
    a_sum += a_data[i];
    b_sum += b_data[i];
    c_sum += c_data[i];
  }

  Phase_Offsets_t result;
  result.a = a_sum /ADC_OFFSET_SAMPLES_COUNT;
  result.b = b_sum /ADC_OFFSET_SAMPLES_COUNT;
  result.c = c_sum /ADC_OFFSET_SAMPLES_COUNT;
  return result;
}

static ADC_Status_t ADC_ValidatePhaseOffsets(Phase_Offsets_t offsets)
{
  if(offsets.a <= ADC_OFFSET_BOTTOM_MARGAIN || offsets.a >= ADC_OFFSET_TOP_MARGAIN)
    return ADC_FAIL;
  if(offsets.b <= ADC_OFFSET_BOTTOM_MARGAIN || offsets.b >= ADC_OFFSET_TOP_MARGAIN)
    return ADC_FAIL;
  if(offsets.c <= ADC_OFFSET_BOTTOM_MARGAIN || offsets.c >= ADC_OFFSET_TOP_MARGAIN)
    return ADC_FAIL;

  return ADC_SUCCESS;
}

//_______________OCP__________________

void ADC_Set_OCP_Threshold(uint16_t thresholdCurrent_ma)
{
  if(ADC_Status & ~FLAG_ADC_OFFSETS_CALIBRATED)
    return;

  uint16_t thresholdOffset = thresholdCurrent_ma*1000/3128;
  uint16_t midpointAvgg    = (phase_offsets.a+phase_offsets.b+phase_offsets.c)/3;
  uint16_t thresholdHigh   = midpointAvgg + thresholdOffset;
  uint16_t thresholdLow    = midpointAvgg - thresholdOffset;

  LL_ADC_ConfigAnalogWDThresholds(ADC_USED, LL_ADC_AWD2, (thresholdHigh>>4), (thresholdLow>>4));
}

void ADC_Set_OCP_ISR(void (*isr)(void))
{
  if(isr == NULL)
    return;

  OverCurrentInterruptCallback = isr;
}

static void LogOverCurrentReason()
{
  uint16_t phase_a, phase_b, phase_c;

  phase_a = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_A_CHANNEL]);
  phase_b = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_B_CHANNEL]);
  phase_c = LL_ADC_INJ_ReadConversionData12(ADC_USED, adc_channel_map[PHASE_C_CHANNEL]);

  Phase_Currents_t currents = ADC_CalculatePhaseCurrents(phase_a, phase_b, phase_c);

  char log[128];
  sprintf(log,"OCP : Trigger : %ld : %ld : %ld : ma", currents.Current_A, currents.Current_B, currents.Current_C);
  LOG(log);
}

/**
  * @brief This function handles ADC_USED global interrupt.
  */
void ADC_USED_IRQHandler(void)
{
  if(LL_ADC_IsActiveFlag_AWD2(ADC_USED))
  {
    if(OverCurrentInterruptCallback != NULL)
      OverCurrentInterruptCallback();
    
    LogOverCurrentReason();
    LL_ADC_ClearFlag_AWD2(ADC_USED);
  }
}
