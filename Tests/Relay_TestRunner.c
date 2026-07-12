#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Relay)
{
    RUN_TEST_CASE(Relay, AfterInitRelayIsOFF);
    RUN_TEST_CASE(Relay, AfterTurningOnRelayIsOn);
    RUN_TEST_CASE(Relay, AfterTurningOffRelayIsOff);
    RUN_TEST_CASE(Relay, VoltageWrongRelayOff);
    RUN_TEST_CASE(Relay, VoltageCorrectRelayOn);
    RUN_TEST_CASE(Relay, ChangeThresholdWrongVoltage);
    RUN_TEST_CASE(Relay, ChangeThresholdCorrectVoltage);
}