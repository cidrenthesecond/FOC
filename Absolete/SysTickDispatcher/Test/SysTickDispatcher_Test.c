#include "unity_fixture.h"
#include "fff.h"

#include "SysTickDispatcher.h"

DEFINE_FFF_GLOBALS;

void FakeTask0();  void FakeTask1();  void FakeTask2();
void FakeTask3();  void FakeTask4();  void FakeTask5();
void FakeTask6();  void FakeTask7();  void FakeTask8();
void FakeTask9();  void FakeTask10();

FAKE_VOID_FUNC(FakeTask0); FAKE_VOID_FUNC(FakeTask1);  FAKE_VOID_FUNC(FakeTask2);
FAKE_VOID_FUNC(FakeTask3); FAKE_VOID_FUNC(FakeTask4);  FAKE_VOID_FUNC(FakeTask5);
FAKE_VOID_FUNC(FakeTask6); FAKE_VOID_FUNC(FakeTask7);  FAKE_VOID_FUNC(FakeTask8);
FAKE_VOID_FUNC(FakeTask9); FAKE_VOID_FUNC(FakeTask10);

#define RESET_ALL_FAKES() \
RESET_FAKE(FakeTask0); RESET_FAKE(FakeTask1); RESET_FAKE(FakeTask2); \
RESET_FAKE(FakeTask3); RESET_FAKE(FakeTask4); RESET_FAKE(FakeTask5); \
RESET_FAKE(FakeTask6); RESET_FAKE(FakeTask7); RESET_FAKE(FakeTask8); \
RESET_FAKE(FakeTask9); RESET_FAKE(FakeTask10); 
    
void (*test_functions[SYS_TICK_DISPATCHER_MAX_TASKS + 1]) = {
    FakeTask0, FakeTask1, FakeTask2,
    FakeTask3, FakeTask4, FakeTask5,
    FakeTask6, FakeTask7, FakeTask8,
    FakeTask9, FakeTask10
};


void ELAPSE_TIME(uint32_t ms)
{
    for(uint32_t ms_elapsed = 0; ms_elapsed < ms; ms_elapsed++)
        SysTickDispatcher_Wakeup();
}

TEST_GROUP(SysTickDispatcher);

TEST_SETUP(SysTickDispatcher)
{
    FFF_RESET_HISTORY();
    RESET_ALL_FAKES();

    SysTickDispatcher_Init();
}

TEST_TEAR_DOWN(SysTickDispatcher)
{
    SysTickDispatcher_Destroy();
}

//_________________SUBSCRIBE_____________________

TEST(SysTickDispatcher, Subscribe_WhenNoInit_ReturnsFail)
{
    SysTickDispatcher_Destroy();

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Subscribe_WhenPeriodIsGreaterThanZero_ReturnsSuccess)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_Subscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Subscribe_WhenPeriodIsZero_ReturnsFail)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(FakeTask0,0));
}

TEST(SysTickDispatcher, Subscribe_WhenTaskIsNull_ReturnsFail)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(NULL,14));
}

TEST(SysTickDispatcher, Subscribe_CorrectArguments_ReturnsSuccess)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_Subscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Subscribe_CanSubscribeMaxTasks)
{
    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
        TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_Subscribe(test_functions[index],index+1));
}

TEST(SysTickDispatcher, Subscribe_WhenOverMaxTasks_ThrowsError)
{
    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
        SysTickDispatcher_Subscribe(test_functions[index],index+1);

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(FakeTask10,25));
}

TEST(SysTickDispatcher, Subscribe_WhenRegisteringSameTask_ThrowsError)
{
    SysTickDispatcher_Subscribe(FakeTask0,24);
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_Subscribe(FakeTask0,23));
}

TEST(SysTickDispatcher, Subscribe_WhenTimerIsAlreadyRunning_StartsTaskPeriodFromSubscription)
{
    ELAPSE_TIME(15);
    SysTickDispatcher_Subscribe(FakeTask0,10);
    ELAPSE_TIME(10);

    TEST_ASSERT_EQUAL(1,FakeTask0_fake.call_count);
}


//_________________UNSUBSCRIBE_____________________

TEST(SysTickDispatcher, Unsubscribe_WhenNotInitialized_ReturnsNoInitError)
{
    SysTickDispatcher_Destroy();
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_INIT, SysTickDispatcher_UnSubscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenNoTasksAreSubscribed_ReturnsNoTasksError)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS, SysTickDispatcher_UnSubscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsSubscribed_ReturnsSuccess)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_UnSubscribe(FakeTask0,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsRemoved_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    SysTickDispatcher_UnSubscribe(FakeTask0,10);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(0,FakeTask0_fake.call_count);
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsNotSubscribed_ReturnsFail)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(FakeTask1,10));
}

TEST(SysTickDispatcher, Unsubscribe_WhenPeriodDoesNotMatch_ReturnsFail)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(FakeTask0,20));
}

TEST(SysTickDispatcher, Unsubscribe_WhenOperationFails_KeepsTaskSubscribed)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    SysTickDispatcher_UnSubscribe(FakeTask0,15);
    ELAPSE_TIME(30);

    TEST_ASSERT_GREATER_THAN(0,FakeTask0_fake.call_count);
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskHasElapsedPartOfPeriod_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FakeTask0, 10);

    ELAPSE_TIME(5);
    SysTickDispatcher_UnSubscribe(FakeTask0, 10);

    ELAPSE_TIME(10);

    TEST_ASSERT_EQUAL(0, FakeTask0_fake.call_count);
}

TEST(SysTickDispatcher, Wakeup_WhenLastTaskIsUnsubscribed_ReturnsNoTasksError)
{
    SysTickDispatcher_Subscribe(FakeTask0, 10);
    SysTickDispatcher_UnSubscribe(FakeTask0, 10);

    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS,SysTickDispatcher_Wakeup());       
}

TEST(SysTickDispatcher, Unsubscribe_WhenTaskIsNull_ReturnFail)
{
    SysTickDispatcher_Subscribe(FakeTask0,5);
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(NULL,5));
}

TEST(SysTickDispatcher, Unsubscribe_WhenPeriodIsZeroAndTaskIsNull_ReturnFail)
{
    SysTickDispatcher_Subscribe(FakeTask0,5);
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_FAIL,SysTickDispatcher_UnSubscribe(NULL,0));
}

TEST(SysTickDispatcher, CanUnsubscribeMAX)
{
    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
        SysTickDispatcher_Subscribe(test_functions[index],index+1);

    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
        TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_SUCCESS,SysTickDispatcher_UnSubscribe(test_functions[index],index+1));
}

//_______________WAKE UP_____________________

TEST(SysTickDispatcher, Wakeup_WhenNotInitialized_ReturnsNoInitError)
{
    SysTickDispatcher_Destroy();
    
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_INIT,SysTickDispatcher_Wakeup());
}

TEST(SysTickDispatcher, Wakeup_WhenNoTasksAreSubscribed_ReturnsNoTasksError)
{
    TEST_ASSERT_EQUAL(SYSTICKDISPATCHER_NO_TASKS,SysTickDispatcher_Wakeup());
}



TEST(SysTickDispatcher, Wakeup_WhenPeriodHasNotElapsed_DoesNotDispatchTask)
{
    SysTickDispatcher_Subscribe(FakeTask0,25);

    ELAPSE_TIME(24);
    TEST_ASSERT_EQUAL(0,FakeTask0_fake.call_count);
}

TEST(SysTickDispatcher,Wakeup_WhenPeriodHasElapsed_DispatchesTask)
{
    SysTickDispatcher_Subscribe(FakeTask0,25);
    ELAPSE_TIME(25);

    TEST_ASSERT_EQUAL(1,FakeTask0_fake.call_count);
}

TEST(SysTickDispatcher, Wakeup_WhenTaskIsSubscribed_DispatchesTaskEveryPeriod)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(10,FakeTask0_fake.call_count);
}



TEST(SysTickDispatcher, Wakeup_WhenMultipleTasksAreSubscribed_DispatchesEachAccordingToItsPeriod)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    SysTickDispatcher_Subscribe(FakeTask1,20);
    SysTickDispatcher_Subscribe(FakeTask2,25);
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(10, FakeTask0_fake.call_count);
    TEST_ASSERT_EQUAL(5,  FakeTask1_fake.call_count);
    TEST_ASSERT_EQUAL(4,  FakeTask2_fake.call_count);
}

void TEST_ASSERT_CALLED_ALL_FUNCTIONS_ATLEAST_ONCE()
{
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask0_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask1_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask2_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask3_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask4_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask5_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask6_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask7_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask8_fake.call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(1,FakeTask9_fake.call_count);

}
TEST(SysTickDispatcher, Wakeup_AllTasksCanBeCalled)
{
    for(uint8_t index = 0; index < SYS_TICK_DISPATCHER_MAX_TASKS; index++)
        SysTickDispatcher_Subscribe(test_functions[index],index+1);

    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(10,SYS_TICK_DISPATCHER_MAX_TASKS); //this is guard if this brakes then forward function is wrong
    TEST_ASSERT_CALLED_ALL_FUNCTIONS_ATLEAST_ONCE();
}

TEST(SysTickDispatcher, Init_WhenCalled_ClearsAllSubscriptions)
{
    SysTickDispatcher_Subscribe(FakeTask0,10);
    SysTickDispatcher_Destroy();
    SysTickDispatcher_Init();
    ELAPSE_TIME(100);

    TEST_ASSERT_EQUAL(0,FakeTask0_fake.call_count);
}
