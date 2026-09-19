#ifndef ADC_SERVICE_H
#define ADC_SERVICE_H

#include <stdint.h>

//Currents are returned in mA
//Voltages are returned in mV

typedef enum
{
    PHASE_A_CHANNEL,
    PHASE_B_CHANNEL,
    PHASE_C_CHANNEL,
    BUS_VOLTAGE_CHANNEL,
    NTC_VOLTAGE_CHANNEL,
} ADC_Channel_t;

typedef struct {
    int32_t Current_A;
    int32_t Current_B;
    int32_t Current_C;
} Phase_Currents_t;

void ADC_Init();

uint16_t ADC_ReadSingleChannelRaw(ADC_Channel_t channel);

uint32_t ADC_GetDcLinkVoltage();
uint32_t ADC_CalculateDcLinkVoltage(uint16_t adcMeasurement);

uint16_t ADC_GetNtcVoltage();
uint16_t ADC_CalculateNtcVoltage(uint16_t adcMeasurement);

Phase_Currents_t  ADC_GetPhaseCurrents();
Phase_Currents_t  ADC_CalculatePhaseCurrents(uint16_t ADC_Phase_A, uint16_t ADC_Phase_B, uint16_t ADC_Phase_C);

void ADC_SetOCP(uint16_t thresholdCurrent_ma);

uint16_t GetNtcVoltage();

#endif
