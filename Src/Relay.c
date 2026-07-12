#include "Relay.h"
#include "ADC_Service.h"


static uint8_t relayState = RELAY_OFF;
static uint8_t relayThresholdVoltage = 230;

void Relay_Init()
{
    relayState = RELAY_OFF;
    relayThresholdVoltage = 230;
}

uint8_t Relay_IsOn()
{
    return relayState;
}

void Relay_TurnOn()
{
    relayState = RELAY_ON;
}

void Relay_SM()
{
    uint16_t DcLinkVoltage = GetDcLinkVoltage();
    
    if(DcLinkVoltage > relayThresholdVoltage)
        Relay_TurnOn();

    //LOG(RELAY ON, DC_Link 230V)
}

void Relay_SetThreshold(uint16_t threshold)
{
    if(threshold > 500)
    {
        //LOG(RELAY SET VALUE INCORRECT)
    }

    relayThresholdVoltage = threshold;
}

void Relay_TurnOff()
{
    relayState = RELAY_OFF;
}