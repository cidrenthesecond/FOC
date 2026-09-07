#include "unity_fixture.h"

TEST_GROUP_RUNNER(Timer)
{
    RUN_TEST_CASE(Timer, CallingWakeupWithoutInitThrowsError);
    RUN_TEST_CASE(Timer, CallingWakeupWithoutTasksThrowsError);
    RUN_TEST_CASE(Timer, SubscribingWithTimeHigherThanZeroReturnsSuccess);
    RUN_TEST_CASE(Timer, SubscribingWithTimeEqualToZeroReturnsError);
    RUN_TEST_CASE(Timer, SuccessfullSubscriptionWrongTimeTaskIsNotCalled);
    RUN_TEST_CASE(Timer,SuccessfullSubscriptionRightTimeTaskIsCalled);
}