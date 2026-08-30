#include "unity_fixture.h"

#include "WaveGen.h"
#include "WaveGen_LUTs.h"

WaveGen UUT;

TEST_GROUP(WaveGen);

TEST_SETUP(WaveGen)
{
    UUT = WaveGen_Create(&SINE);
}

TEST_TEAR_DOWN(WaveGen)
{
    WaveGen_Destroy(UUT);
}

TEST(WaveGen, WhenNoFrequencyIsSetGettingReturnsZeroes)
{
    TEST_ASSERT_EQUAL(0,WaveGen_Get(UUT));
    TEST_ASSERT_EQUAL(0,WaveGen_Get(UUT));
}

TEST(WaveGen, ReturnNULLWhenLutSizeIsNotPowerOfTwo)
{
    WaveGen_Destroy(UUT);
    const WaveGen_LUT dummy = {
        .data = NULL,
        .size = 15
    };

    UUT = WaveGen_Create(&dummy);

    TEST_ASSERT_EQUAL_PTR(NULL,UUT);
}