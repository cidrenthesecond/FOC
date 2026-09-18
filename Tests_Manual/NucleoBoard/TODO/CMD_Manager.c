#include "stdint.h"
#include "stdint.h"
#include "ADC_Service.h"
#include "Thermistor.h"
#include "Logger.h"
#include "stdlib.h"
#include <stdio.h>

void ADC_Measure(int argc, char *argv[])
{
    if(argc == 0 || argc > 1)
        return;

    uint32_t argument = strtol(argv[0],NULL,10);
    
    switch (argument)
    {
        case 0:
        {
            uint16_t measurementRaw = ADC_ReadSingleChannelRaw(BUS_VOLTAGE_CHANNEL);
            uint32_t voltage = ADC_CalculateDcLinkVoltage(measurementRaw);

            char message[64];
            sprintf(message, "[ADC] : DC Bus Voltage : %u -> %lu mV",measurementRaw,voltage);
            LOG(message);

            break;
        }
        case 1:
        {
            uint16_t measurementRaw   = ADC_ReadSingleChannelRaw(PHASE_A_CHANNEL);
            Phase_Currents_t currents = ADC_CalculatePhaseCurrents(measurementRaw, 0, 0);

            char message[64];
            sprintf(message, "[ADC] : Phase A current : %u -> %ld mA",measurementRaw,currents.Current_A);
            LOG(message);
            break;
        }
        case 2:
        {
            uint16_t measurementRaw   = ADC_ReadSingleChannelRaw(PHASE_B_CHANNEL);
            Phase_Currents_t currents = ADC_CalculatePhaseCurrents(0, measurementRaw, 0);

            char message[64];
            sprintf(message, "[ADC] : Phase B current : %u -> %ld mA",measurementRaw,currents.Current_B);
            LOG(message);
            break;
        }
        case 3:
        {
            uint16_t measurementRaw   = ADC_ReadSingleChannelRaw(PHASE_C_CHANNEL);
            Phase_Currents_t currents = ADC_CalculatePhaseCurrents(0, 0, measurementRaw);

            char message[64];
            sprintf(message, "[ADC] : Phase C current : %u -> %ld mA",measurementRaw,currents.Current_C);
            LOG(message);
            break;
        }
        case 4:
        {
            uint16_t measurementRaw = ADC_ReadSingleChannelRaw(NTC_VOLTAGE_CHANNEL);
            uint32_t voltage        = ADC_CalculateNtcVoltage(measurementRaw);

            char message[64];
            sprintf(message, "[ADC] : NTC Voltage : %u -> %lu mV",measurementRaw,voltage);
            LOG(message);
            
            break;
        }
        default:
            break;
    }
}

void NTC_Measure(int argc, char *argv[])
{
    int16_t Temp = Thermistor_GetHeatsinkTemp();

    char message[32];
    sprintf(message, "[NTC] : HT Temp : %d",Temp);
    LOG(message);
}
