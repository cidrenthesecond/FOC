#include "stdint.h"
#include "stdint.h"
#include "ADC_Service.h"
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
            sprintf(message, "[ADC] : DC Bus Voltage : %u -> %u mV",measurementRaw,voltage);
            LOG(message);

            break;
        }
        case 1:
        {
            uint16_t measurementRaw = ADC_ReadSingleChannelRaw(NTC_VOLTAGE_CHANNEL);
            uint32_t voltage        = ADC_CalculateNtcVoltage(measurementRaw);

            char message[64];
            sprintf(message, "[ADC] : NTC Voltage : %u -> %u mV",measurementRaw,voltage);
            LOG(message);
            
            break;
        }
        default:
            break;
    }

    

}
