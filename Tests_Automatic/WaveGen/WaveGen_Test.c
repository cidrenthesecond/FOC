#include "unity_fixture.h"
#include "WaveGen.h"
#include "WaveGen_LUTs.h"

static const int32_t data1[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
static const int32_t data2[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const WaveGen_LUT LUT1 = {
    .data = data1,
    .size = 8
};
const WaveGen_LUT LUT2 = {
    .data = data2,
    .size = 16
};
const WaveGen_LUT Improper_LUT = {
    .data = NULL,
    .size = 15
 };

 WaveGen UUT;

TEST_GROUP(WaveGen);

TEST_SETUP(WaveGen)
{
    UUT = WaveGen_Create(&LUT1);
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

    UUT = WaveGen_Create(&Improper_LUT);

    TEST_ASSERT_EQUAL_PTR(NULL,UUT);
}

TEST(WaveGen, WhenChangeToImproperLutThrowError)
{
    uint8_t errorCode = 0xFF;
    TEST_ASSERT_EQUAL(errorCode,WaveGen_SetDesiredWave(UUT,&Improper_LUT));
}

TEST(WaveGen, WaveGenTakesDataFromDesiredWave)
{
    WaveGen_SetFrequency(UUT,10.0f,20000.0f);

    TEST_ASSERT_EQUAL(data1[0],WaveGen_Get(UUT));

    WaveGen_SetDesiredWave(UUT,&LUT2);
    WaveGen_SetFrequency(UUT,10.0f,20000.0f);

    TEST_ASSERT_EQUAL(data2[0],WaveGen_Get(UUT));
}



