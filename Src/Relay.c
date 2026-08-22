#include "Relay.h"
#include "GPIO_Driver.h"

#include "ADC_Service.h"
#include "Logger.h"
#include "stdio.h"

static uint8_t relayState;
static uint8_t relayThresholdVoltage = 230;

void Relay_Init()
{
    Relay_TurnOff();
    relayThresholdVoltage = 230;
}

void Relay_SM()
{
    uint16_t DcLinkVoltage = GetDcLinkVoltage();
    
    if(DcLinkVoltage > relayThresholdVoltage)
        Relay_TurnOn();

    char log[30];
    sprintf(log,"RELAY : ON : %uV\n",DcLinkVoltage);
    LOG(log);
}

void Relay_SetThreshold(uint16_t threshold)
{
    if(threshold > 500)
    {
        //LOG(RELAY SET VALUE INCORRECT)
    }

    relayThresholdVoltage = threshold;
}

uint8_t Relay_IsOn()
{
    return relayState;
}

void Relay_TurnOn()
{
    relayState = RELAY_ON;
    GPIO_Relay_TurnOn();
}

void Relay_TurnOff()
{
    relayState = RELAY_OFF;
    GPIO_Relay_TurnOff();
}