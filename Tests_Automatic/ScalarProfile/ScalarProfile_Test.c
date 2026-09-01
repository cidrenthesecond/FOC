#include "unity_fixture.h"
#include "ScalarProfile.h"

TEST_GROUP(ScalarProfile);

static float testRatedVoltage = 230.0f;
static float testBoostVoltage = 50.0f;
static float testRatedFrequency = 50.0f;
static float testBoostFrequency = 10.0f;

TEST_SETUP(ScalarProfile)
{
    ScalarProfile_Init(testRatedVoltage,testBoostVoltage,testRatedFrequency,testBoostFrequency);
}

TEST_TEAR_DOWN(ScalarProfile)
{
    ScalarProfile_Destroy();
}

TEST(ScalarProfile,WhenNoInitAlwaysReturnsZero)
{
    ScalarProfile_Destroy();

    TEST_ASSERT_EQUAL_FLOAT(0.0f,ScalarProfile_GetVoltage(15.0f));
}

TEST(ScalarProfile,WhenFrequencyIsBelowBoostReturnBoostVoltage)
{
    TEST_ASSERT_EQUAL(testBoostVoltage, ScalarProfile_GetVoltage(testBoostFrequency - 1.0f));
}

TEST(ScalarProfile,WhenFrequencyIsEqualBoostReturnBoostVoltage)
{
    TEST_ASSERT_EQUAL(testBoostVoltage, ScalarProfile_GetVoltage(testBoostFrequency));
}

TEST(ScalarProfile,WhenFrequencyIsAboveRatedReturnRatedVoltage)
{
    TEST_ASSERT_EQUAL(testRatedVoltage, ScalarProfile_GetVoltage(testRatedFrequency + 1.0f));
}

TEST(ScalarProfile, WhenFrequencyIsEqualRatedReturnRatedVoltage)
{
    TEST_ASSERT_EQUAL(testRatedVoltage, ScalarProfile_GetVoltage(testRatedFrequency));
}

TEST(ScalarProfile, WhenFrequencyIsBetweenBoostAndRatedInterpolateVoltage)
{
    TEST_ASSERT_GREATER_THAN_FLOAT(testBoostVoltage,ScalarProfile_GetVoltage(testBoostFrequency + 5.0f));
    TEST_ASSERT_LESS_THAN_FLOAT(testRatedVoltage, ScalarProfile_GetVoltage(testBoostFrequency + 10.0f));
}