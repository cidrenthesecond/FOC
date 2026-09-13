#include "unity_fixture.h"

TEST_GROUP_RUNNER(SysTickDispatcher)
{
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenNotInitialized_ReturnsNoInitError);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenNoTasksAreSubscribed_ReturnsNoTasksError);
    RUN_TEST_CASE(SysTickDispatcher, Subscribe_WhenPeriodIsGreaterThanZero_ReturnsSuccess);
    RUN_TEST_CASE(SysTickDispatcher, Subscribe_WhenPeriodIsZero_ReturnsFail);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenPeriodHasNotElapsed_DoesNotDispatchTask);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenPeriodHasElapsed_DispatchesTask);
    RUN_TEST_CASE(SysTickDispatcher, Subscribe_WhenTimerIsAlreadyRunning_StartsTaskPeriodFromSubscription);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenTaskIsSubscribed_DispatchesTaskEveryPeriod);
    RUN_TEST_CASE(SysTickDispatcher, Init_WhenCalled_ClearsAllSubscriptions);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenNotInitialized_ReturnsNoInitError);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenNoTasksAreSubscribed_ReturnsNoTasksError);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenTaskIsSubscribed_ReturnsSuccess);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenTaskIsRemoved_DoesNotDispatchTask);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenTaskIsNotSubscribed_ReturnsFail);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenPeriodDoesNotMatch_ReturnsFail);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenOperationFails_KeepsTaskSubscribed);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenMultipleTasksAreSubscribed_DispatchesEachAccordingToItsPeriod);
    RUN_TEST_CASE(SysTickDispatcher, Unsubscribe_WhenTaskHasElapsedPartOfPeriod_DoesNotDispatchTask);
    RUN_TEST_CASE(SysTickDispatcher, Wakeup_WhenLastTaskIsUnsubscribed_ReturnsNoTasksError);
}