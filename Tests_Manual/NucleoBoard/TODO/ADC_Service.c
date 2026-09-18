#include "stm32h5xx_ll_adc.h"
#include "ADC_Service.h"
#include "Moving_Avarage_Filter.h"

#include "Logger.h"
#include "stdio.h"

#define NTC_CHANNEL     LL_ADC_CHANNEL_0
#define DC_LINK_CHANNEL LL_ADC_CHANNEL_1
#define PHASE_A_CHANNEL LL_ADC_CHANNEL_3
#define PHASE_B_CHANNEL LL_ADC_CHANNEL_4
#define PHASE_C_CHANNEL LL_ADC_CHANNEL_7

#define ADC_FULL_SCALE           4095
#define ADC_VREF_MV              3300
#define DIVIDER_CONSTANT         126  // do przeliczenia jeszcze raz i zmiana nazwy

#define ADC_OFFSET_SAMPLES_COUNT 5U
#define FILTER_KERNEL 3

typedef struct{
  uint16_t raw;
  uint16_t filtered;
} VoltageMeasurement_t;

typedef struct{
  int16_t raw_delta;
  int16_t filtered_delta;
} CurrentMeasurement_t;

VoltageMeasurement_t Bus_Voltage;
VoltageMeasurement_t NTC_Voltage;
CurrentMeasurement_t A_Current;
CurrentMeasurement_t B_Current;
CurrentMeasurement_t C_Current;  

static uint16_t bus_Measurement;
static uint16_t NTC_Measurement;
static uint16_t phase_A_Measurement;
static uint16_t phase_B_Measurement;
static uint16_t phase_C_Measurement;

static uint16_t phase_a_offset_adc;
static uint16_t phase_b_offset_adc;
static uint16_t phase_c_offset_adc;

static MovingAvarage NTC_Filter;
static MovingAvarage DcBus_Filter;
static MovingAvarage Phase_A_Filter;
static MovingAvarage Phase_B_Filter;
static MovingAvarage Phase_C_Filter;

typedef union {
    uint8_t raw;
    struct {
        uint8_t ready     : 1; // Bit 0
        uint8_t error     : 1; // Bit 1
        uint8_t busy      : 1; // Bit 2
        uint8_t tx_active : 1; // Bit 3
        uint8_t reserved  : 4; // Bity 4-7
    } bits;
} StatusRegister_t;

static volatile StatusRegister_t status = {0};

static void ADC_CalibratePhaseOffsets(void);
static void ADC_PrepareForPhaseOffsetMeasurement(void);
static void ADC_GatherOffsetData(uint16_t * a_data,uint16_t * b_data, uint16_t * c_data);
static void ADC_CalculatePhaseOffsets(uint16_t *a_data, uint16_t *b_data, uint16_t * c_data);
static void ADC_Calibrate();

static float CalculatePhaseCurrent(int16_t measurement);

static uint32_t ADC_CalculateDcLinkVoltage(uint16_t adcMeasurement);

void ADC_Init()
{
  NTC_Filter     = MovingAvarage_Init(FILTER_KERNEL);
  DcBus_Filter   = MovingAvarage_Init(FILTER_KERNEL);
  Phase_A_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_B_Filter = MovingAvarage_Init(FILTER_KERNEL);
  Phase_C_Filter = MovingAvarage_Init(FILTER_KERNEL);

  ADC_Calibrate();
  ADC_CalibratePhaseOffsets();

  //LL_ADC_EnableIT_JEOS(ADC1); NOT ADDED IN EXTI FOR NOW
  LL_ADC_Enable(ADC1);
  LL_ADC_INJ_StartConversion(ADC1);
}

static uint16_t ADC_ReadSingleChannelRaw(uint32_t channel)
{
    //TO DO: Czy tutaj nie musze zamienic kanalow miejscami?
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, channel);

    LL_ADC_REG_StartConversion(ADC1);
    while (!LL_ADC_IsActiveFlag_EOC(ADC1))
        ;
    LL_ADC_ClearFlag_EOC(ADC1);

    return LL_ADC_REG_ReadConversionData12(ADC1);
}

static void ADC_Calibrate()
{
  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);

  while(LL_ADC_IsCalibrationOnGoing(ADC1))
	  ;
}

uint32_t ADC_GetDcLinkVoltage()
{
  // uint16_t measurement = ADC_ReadSingleChannelRaw(DC_LINK_CHANNEL);
  // bus_Measurement      = MovingAvarage_Filter(DcBus_Filter, measurement);
  return ADC_CalculateDcLinkVoltage(Bus_Voltage.filtered);
}

uint32_t ADC_CalculateDcLinkVoltage(uint16_t adcMeasurement)
{
	return DIVIDER_CONSTANT * ADC_VREF_MV * adcMeasurement / ADC_FULL_SCALE;
}

uint16_t ADC_GetNtcVoltage()
{
  uint16_t measurement = ADC_ReadSingleChannelRaw(NTC_CHANNEL);
  NTC_Measurement      = MovingAvarage_Filter(NTC_Filter, measurement);
  return NTC_Measurement*ADC_VREF_MV / ADC_FULL_SCALE;
}

uint16_t ADC_GetPhaseARawCurrentMeasurement()
{
  return ADC_ReadSingleChannelRaw(PHASE_A_CHANNEL);
}

uint16_t ADC_GetPhaseBRawCurrentMeasurement()
{
  return ADC_ReadSingleChannelRaw(PHASE_B_CHANNEL);
}

uint16_t ADC_GetPhaseCRawCurrentMeasurement()
{
  return ADC_ReadSingleChannelRaw(PHASE_C_CHANNEL);
}

static void ADC_CalibratePhaseOffsets(void)
{
  ADC_PrepareForPhaseOffsetMeasurement();

  uint16_t a_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t b_data[ADC_OFFSET_SAMPLES_COUNT];
  uint16_t c_data[ADC_OFFSET_SAMPLES_COUNT];

  ADC_GatherOffsetData(a_data,b_data,c_data);
  ADC_CalculatePhaseOffsets(a_data, b_data, c_data);

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

static float CalculatePhaseCurrent(int16_t delta)
{
  return (float)delta*3.128f;
}

void PrintCurrent_B()
{
  uint16_t adc_measurement = ADC_ReadSingleChannelRaw(LL_ADC_CHANNEL_4);//LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
  int16_t delta = (int16_t)adc_measurement - phase_b_offset_adc;
  int16_t value = delta *3;

  char buffer1[30] = {0};
  sprintf(buffer1,"B : %d",value);
  LOG(buffer1);
}

void PrintCurrent_B_ma()
{
  uint16_t adc_measurement = ADC_ReadSingleChannelRaw(LL_ADC_CHANNEL_4);//LL_ADC_INJ_ReadConversionData12(ADC1, LL_ADC_INJ_RANK_3);
  int16_t delta = (int16_t)adc_measurement - phase_b_offset_adc;
  int16_t value = delta *3128/1000;

  char buffer1[30] = {0};
  sprintf(buffer1,"B : %d",value);
  LOG(buffer1);
}
void PrintFloat(float x);

void PrintCurrent_B_float()
{
  uint16_t measurement = ADC_ReadSingleChannelRaw(LL_ADC_CHANNEL_4);
  int16_t delta = (int16_t)measurement - phase_b_offset_adc;
  float current = CalculatePhaseCurrent(delta);
  PrintFloat(current);
}

void PrintFloat(float x)
{
  int decimal    = (int)x;
  int fractional = (int)((x - decimal)*100);
  char buffer[30];
  sprintf(buffer, "B: %d.%02d", decimal,fractional);
  LOG(buffer);
}


//PRZERWANIE KTORE LICZY prady