#include "unity_fixture.h"

TEST_GROUP_RUNNER(WaveGen)
{
    RUN_TEST_CASE(WaveGen, WhenNoFrequencyIsSetGettingReturnsZeroes);
    RUN_TEST_CASE(WaveGen, ReturnNULLWhenLutSizeIsNotPowerOfTwo);
}