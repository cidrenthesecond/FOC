#include "unity_fixture.h"

#include "WaveGen.h"

WaveGen UUT;

TEST_GROUP(WaveGen);

TEST_SETUP(WaveGen)
{
    UUT = WaveGen_Create();
}

TEST_TEAR_DOWN(WaveGen)
{
    WaveGen_Destroy(UUT);
}

TEST(WaveGen, start)
{
    TEST_ASSERT_TRUE(1);
}