#ifndef ADC_SERVICE_H
#define ADC_SERVICE_H

#include <stdint.h>

void ADC_Init();
uint32_t ADC_GetDcLinkVoltage();
uint16_t ADC_GetRawDcLinkMeasurement();

uint16_t ADC_GetNtcVoltage();
uint16_t ADC_GetRawNtcMeasurement();

uint16_t ADC_GetPhaseARawCurrentMeasurement();
uint16_t ADC_GetPhaseBRawCurrentMeasurement();
uint16_t ADC_GetPhaseCRawCurrentMeasurement();

uint16_t GetDcLinkVoltage();
uint16_t GetNtcVoltage();

#endif
