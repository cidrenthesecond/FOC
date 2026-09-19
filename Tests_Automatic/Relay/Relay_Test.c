#include "unity.h"
#include "unity_fixture.h"

#include "fff.h"
#include "LOG_Spy.h"
#include "GPIO_Driver.h"

#include "Relay.h"

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(uint32_t, ADC_GetDcLinkVoltage);

TEST_GROUP(Relay);

TEST_SETUP(Relay)
{   
    LOG_Spy_Init();
    RESET_FAKE(ADC_GetDcLinkVoltage);
    FFF_RESET_HISTORY();

    Relay_Init();
}

TEST_TEAR_DOWN(Relay)
{

}

TEST(Relay, AfterInitRelayIsOFF)
{
    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, AfterTurningOnRelayIsOn)
{
    Relay_TurnOn();
    TEST_ASSERT_EQUAL(RELAY_ON, Relay_IsOn());
}

TEST(Relay, AfterTurningOffRelayIsOff)
{
    Relay_TurnOn();
    Relay_TurnOff();
    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, VoltageWrongRelayOff)
{
    ADC_GetDcLinkVoltage_fake.return_val = 30000;
    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, VoltageWrongNoLog)
{
    ADC_GetDcLinkVoltage_fake.return_val = 30000;
    Relay_SM();

    TEST_ASSERT_EQUAL(0,LOG_Spy_GetCallCount());
}

TEST(Relay, VoltageCorrectRelayOn)
{
    ADC_GetDcLinkVoltage_fake.return_val = 230000;

    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_ON, Relay_IsOn());
}

TEST(Relay, ChangeThresholdWrongVoltage)
{
    ADC_GetDcLinkVoltage_fake.return_val = 22;
    Relay_SetThreshold(150);

    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, ChangeThresholdCorrectVoltage)
{
    ADC_GetDcLinkVoltage_fake.return_val = 22;
    Relay_SetThreshold(12);

    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_ON, Relay_IsOn());
}

TEST(Relay, RelayTurnOnProducesLog)
{
    ADC_GetDcLinkVoltage_fake.return_val = 240000;

    Relay_SM();

    TEST_ASSERT_EQUAL(1,LOG_Spy_GetCallCount());
}

TEST(Relay, LogFormatting)
{
    ADC_GetDcLinkVoltage_fake.return_val = 240000;
    
    Relay_SM();

    TEST_ASSERT_EQUAL(1,LOG_Spy_GetCallCount());
    TEST_ASSERT_EQUAL_CHAR_ARRAY("RELAY : ON : 240000 mV\n",LOG_Spy_GetMessage(),19);
}