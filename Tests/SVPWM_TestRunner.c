#include "unity_fixture.h"

TEST_GROUP_RUNNER(SVPWM)
{
    RUN_TEST_CASE(SVPWM, AfterInitAllPhasesAreSetToZero);
    RUN_TEST_CASE(SVPWM, AfterDestroyAllPhasesAreSetToZero);
    RUN_TEST_CASE(SVPWM, ZeroVoltage);
}