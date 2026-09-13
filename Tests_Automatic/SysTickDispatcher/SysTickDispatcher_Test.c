#include "unity_fixture.h"
#include "fff.h"

#include "SysTickDispatcher.h"

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
        SysTickDispatcher_Wakeup();
}

TEST_GROUP(SysTickDispatcher);

TEST_SETUP(SysTickDispatcher)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(FirstFakeFunction);
    RESET_FAKE(SecondFakeFunction);
    RESET_FAKE(ThirdFakeFunction);

    SysTickDispatcher_Init();
}

TEST_TEAR_DOWN(SysTickDispatcher)
{
    SysTickDispatcher_Destroy();
}

TEST(SysTickDispatcher, Wakeup_WhenNotInitialized_ReturnsNoInitError)
{
    SysTickDispatcher_Destroy();
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_INIT,SysTickDispatcher_Wakeup());
}

TEST(SysTickDispatcher, Wakeup_WhenNoTasksAreSubscribed_ReturnsNoTasksError)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS,SysTickDispatcher_Wakeup());
}

TEST(SysTickDispatcher, Subscribe_WhenPeriodIsGreaterThanZero_ReturnsSuccess)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_Subscribe(FirstFakeFunction,10));
}

TEST(SysTickDispatcher, Subscribe_WhenPeriodIsZero_ReturnsFail)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(FirstFakeFunction,0));
}

TEST(SysTickDispatcher, Wakeup_WhenPeriodHasNotElapsed_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,25);

    ELAPSE_TIME(24);
    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher,Wakeup_WhenPeriodHasElapsed_DispatchesTask)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,25);
    ELAPSE_TIME(25);

    TEST_ASSERT_EQUAL(1,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Subscribe_WhenTimerIsAlreadyRunning_StartsTaskPeriodFromSubscription)
{
    ELAPSE_TIME(15);
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    ELAPSE_TIME(10);

    TEST_ASSERT_EQUAL(1,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Wakeup_WhenTaskIsSubscribed_DispatchesTaskEveryPeriod)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(10,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Init_WhenCalled_ClearsAllSubscriptions)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    SysTickDispatcher_Destroy();
    SysTickDispatcher_Init();
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Unsubscribe_WhenNotInitialized_ReturnsNoInitError)
{
    SysTickDispatcher_Destroy();
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_INIT, SysTickDispatcher_UnSubscribe(FirstFakeFunction,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenNoTasksAreSubscribed_ReturnsNoTasksError)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS, SysTickDispatcher_UnSubscribe(FirstFakeFunction,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsSubscribed_ReturnsSuccess)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_UnSubscribe(FirstFakeFunction,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsRemoved_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    SysTickDispatcher_UnSubscribe(FirstFakeFunction,10);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(0,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsNotSubscribed_ReturnsFail)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(SecondFakeFunction,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenPeriodDoesNotMatch_ReturnsFail)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(FirstFakeFunction,20));
}

TEST(SysTickDispatcher, Unsubscribe_WhenOperationFails_KeepsTaskSubscribed)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    SysTickDispatcher_UnSubscribe(FirstFakeFunction,15);
    ELAPSE_TIME(30);

    TEST_ASSERT_GREATER_THAN(0,FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Wakeup_WhenMultipleTasksAreSubscribed_DispatchesEachAccordingToItsPeriod)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction,10);
    SysTickDispatcher_Subscribe(SecondFakeFunction,20);
    SysTickDispatcher_Subscribe(ThirdFakeFunction,25);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(10, FirstFakeFunction_fake.call_count);
    TEST_ASSERT_EQUAL(5,  SecondFakeFunction_fake.call_count);
    TEST_ASSERT_EQUAL(4,  ThirdFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskHasElapsedPartOfPeriod_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction, 10);

    ELAPSE_TIME(5);
    SysTickDispatcher_UnSubscribe(FirstFakeFunction, 10);

    ELAPSE_TIME(10);

    TEST_ASSERT_EQUAL(0, FirstFakeFunction_fake.call_count);
}

TEST(SysTickDispatcher, Wakeup_WhenLastTaskIsUnsubscribed_ReturnsNoTasksError)
{
    SysTickDispatcher_Subscribe(FirstFakeFunction, 10);
    SysTickDispatcher_UnSubscribe(FirstFakeFunction, 10);

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS,SysTickDispatcher_Wakeup());       
}