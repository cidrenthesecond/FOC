#include "Thermistor.h"
#include "Thermistor_LUT.h"

#include "ADC_Service.h"

static const uint16_t maxVoltage = 3500;


int16_t Thermistor_GetHeatsinkTemp()
{   
    uint16_t voltageOnNTC = GetNtcVoltage();

    if(voltageOnNTC == 0)
        return NTC_TOO_LOW_VOLTAGE;

    if(voltageOnNTC > maxVoltage)
        return NTC_TOO_HIGH_VOLTAGE;

    for(uint8_t index = 0; index < NTC_LUT_ENTRIES - 1; index++)
    {
        if(NTC_LUT[index].NTC_Voltage == voltageOnNTC)
            return NTC_LUT[index].temperature;

        if(NTC_LUT[index].NTC_Voltage < voltageOnNTC && NTC_LUT[index + 1].NTC_Voltage > voltageOnNTC)
        {
            int32_t voltageDelta     = NTC_LUT[index + 1].NTC_Voltage - NTC_LUT[index].NTC_Voltage;
            int32_t temperatureDelta = NTC_LUT[index + 1].temperature - NTC_LUT[index].temperature;
            int32_t voltageOffset    = voltageOnNTC - NTC_LUT[index].NTC_Voltage;

            int32_t temperatureOffset = (voltageOffset * temperatureDelta) / voltageDelta;

            return NTC_LUT[index].temperature + temperatureOffset;
        }
    }

    if(voltageOnNTC == NTC_LUT[NTC_LUT_ENTRIES - 1].NTC_Voltage)
    {
        return NTC_LUT[NTC_LUT_ENTRIES - 1].temperature;
    }

    return NTC_TOO_HIGH_VOLTAGE;
}