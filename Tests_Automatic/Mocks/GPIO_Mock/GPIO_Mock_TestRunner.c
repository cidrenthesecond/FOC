#include "unity_fixture.h"

TEST_GROUP_RUNNER(GPIO_MOCK)
{
    RUN_TEST_CASE(GPIO_MOCK, AfterInitRelayStateIsUndefined);
    RUN_TEST_CASE(GPIO_MOCK, AfterTurningOnRelayOnStateIsReturned);
    RUN_TEST_CASE(GPIO_MOCK, AfterTurningOffRelayOffStateIsReturned);
}