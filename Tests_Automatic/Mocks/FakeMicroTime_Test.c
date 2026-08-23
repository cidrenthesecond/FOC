#include "unity.h"
#include "unity_fixture.h"

#include "FakeMicroTime.h"

TEST_GROUP(FakeMicroTime);

TEST_SETUP(FakeMicroTime)
{
    FakeMicroTime_Init(0, 5);
}

TEST_TEAR_DOWN(FakeMicroTime)
{

}

TEST(FakeMicroTime, Create)
{
  LONGS_EQUAL(0, MicroTime_Get());
}

TEST(FakeMicroTime, MicroTimeIncrements)
{
    LONGS_EQUAL(0, MicroTime_Get());
    LONGS_EQUAL(5, MicroTime_Get());
    LONGS_EQUAL(10, MicroTime_Get());
}

TEST(FakeMicroTime, MicroTimeDelay)
{
    LONGS_EQUAL(0, MicroTime_Get());
    LONGS_EQUAL(5, MicroTime_Get());
    MicroTime_Delay(20);
    LONGS_EQUAL(30, MicroTime_Get());
}

TEST(FakeMicroTime, MicroTimeDelayRecording)
{
    MicroTime_Delay(20);
    MicroTime_Delay(20);
    MicroTime_Delay(20);
    LONGS_EQUAL(60, FakeMicroTime_GetDelayDuration());
}

TEST(FakeMicroTime, MicroTimeIsSettable)
{
    MicroTime_Set(100);

    LONGS_EQUAL(100, MicroTime_Get());
}