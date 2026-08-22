#include "unity_fixture.h"

#include "GPIO_Mock.h"

TEST_GROUP(GPIO_MOCK);

TEST_SETUP(GPIO_MOCK)
{
    GPIO_Mock_init();
}

TEST_TEAR_DOWN(GPIO_MOCK)
{

}

TEST(GPIO_MOCK, AfterInitRelayStateIsUndefined)
{
    TEST_ASSERT_EQUAL(GPIO_STATE_UNDEFINED,GPIO_Relay_GetState());
}

TEST(GPIO_MOCK, AfterTurningOnRelayOnStateIsReturned)
{
    GPIO_Relay_TurnOn();
    
    TEST_ASSERT_EQUAL(GPIO_STATE_ON,GPIO_Relay_GetState());
}

TEST(GPIO_MOCK, AfterTurningOffRelayOffStateIsReturned)
{
    GPIO_Relay_TurnOff();

    TEST_ASSERT_EQUAL(GPIO_STATE_OFF,GPIO_Relay_GetState());
}