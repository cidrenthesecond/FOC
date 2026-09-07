#include "unity_fixture.h"
#include "fff.h"

#include "Timer.h"

void FirstFakeFunction();
void SecondFakeFunction();
void ThirdFakeFunction();

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(FirstFakeFunction);
FAKE_VOID_FUNC(SecondFakeFunction);
FAKE_VOID_FUNC(ThirdFakeFunction);

void ELAPSE_TIME(uint32_t ms)
{
    for(uint32_t ms_elapsed = 0; ms_elapsed < ms; ms_elapsed++)
        Timer_Wakeup();
}

TEST_GROUP(Timer);

TEST_SETUP(Timer)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(FirstFakeFunction);
    RESET_FAKE(SecondFakeFunction);
    RESET_FAKE(ThirdFakeFunction);

    Timer_Init();
}

TEST_TEAR_DOWN(Timer)
{

}

TEST(Timer, CallingWakeupWithoutInitThrowsError)
{
    Timer_Destroy();
    
    TEST_ASSERT_EQUAL(TIMER_NO_INIT,Timer_Wakeup());
}

TEST(Timer, CallingWakeupWithoutTasksThrowsError)
{
    TEST_ASSERT_EQUAL(TIMER_NO_TASKS,Timer_Wakeup());
}

TEST(Timer, SubscribingWithTimeHigherThanZeroReturnsSuccess)
{
    TEST_ASSERT_EQUAL(TIMER_SUCCESS,Timer_Subscribe(FirstFakeFunction,10));
}

TEST(Timer, SubscribingWithTimeEqualToZeroReturnsError)
{
    TEST_ASSERT_EQUAL(TIMER_FAIL,Timer_Subscribe(FirstFakeFunction,0));
}

TEST(Timer, SuccessfullSubscriptionWrongTimeTaskIsNotCalled)
{
    Timer_Subscribe(FirstFakeFunction,25);

    Timer_Wakeup();
    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
    Timer_Wakeup();
    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
    Timer_Wakeup();
    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
}

TEST(Timer,SuccessfullSubscriptionRightTimeTaskIsCalled)
{
    Timer_Subscribe(FirstFakeFunction,25);
    ELAPSE_TIME(25);
    Timer_Wakeup();

    TEST_ASSERT_EQUAL(1,FirstFakeFunction_fake.call_count);
}