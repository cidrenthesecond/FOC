#include "FakeDcLink.h"

static uint16_t supplyVoltage = 0;

void Fake_SetDcLinkVoltage(uint16_t voltage)
{
    supplyVoltage = voltage;
}

uint16_t GetDcLinkVoltage()
{
    return supplyVoltage;
}

// uint16_t Fake_GetDcLinkVoltage()
// {
//     return supplyVoltage;
// }



void Fake_ResetDcLinkVoltage()
{
    supplyVoltage = 0;
}