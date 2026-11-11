#include "unity_fixture.h"

TEST_GROUP_RUNNER(ScalarProfile)
{
    RUN_TEST_CASE(ScalarProfile,WhenNoInitAlwaysReturnsZero);
    RUN_TEST_CASE(ScalarProfile,WhenFrequencyIsBelowBoostReturnBoostVoltage);
    RUN_TEST_CASE(ScalarProfile,WhenFrequencyIsEqualBoostReturnBoostVoltage);
    RUN_TEST_CASE(ScalarProfile,WhenFrequencyIsAboveRatedReturnRatedVoltage);
    RUN_TEST_CASE(ScalarProfile,WhenFrequencyIsEqualRatedReturnRatedVoltage);
    RUN_TEST_CASE(ScalarProfile, WhenFrequencyIsBetweenBoostAndRatedInterpolateVoltage);
}