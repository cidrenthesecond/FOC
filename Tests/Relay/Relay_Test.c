#include "unity.h"
#include "unity_fixture.h"

#include "FakeDcLink.h"
#include "Logger.h"
#include "fff.h"

#include "Relay.h"

TEST_GROUP(Relay);

TEST_SETUP(Relay)
{   
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
    Fake_SetDcLinkVoltage(12);
    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, VoltageCorrectRelayOn)
{
    Fake_SetDcLinkVoltage(240);
    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_ON, Relay_IsOn());
}

TEST(Relay, ChangeThresholdWrongVoltage)
{
    Fake_SetDcLinkVoltage(22);
    Relay_SetThreshold(150);
    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_OFF, Relay_IsOn());
}

TEST(Relay, ChangeThresholdCorrectVoltage)
{
    Fake_SetDcLinkVoltage(22);
    Relay_SetThreshold(12);
    Relay_SM();

    TEST_ASSERT_EQUAL(RELAY_ON, Relay_IsOn());
}

TEST(Relay, RelayTurnOnProducesLog)
{
    Fake_SetDcLinkVoltage(240);
    Relay_SM();

    TEST_ASSERT_EQUAL(1,LOG_fake.call_count);
}

TEST(Relay, LogFormatting)
{
    Fake_SetDcLinkVoltage(240);
    Relay_SM();

    TEST_ASSERT_EQUAL(1,LOG_fake.call_count);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("RELAY : ON : 240V\n",LOG_fake.arg0_val,19);
}