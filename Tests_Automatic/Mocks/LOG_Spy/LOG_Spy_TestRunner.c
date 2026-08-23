#include "unity_fixture.h"

TEST_GROUP_RUNNER(LOG_Spy)
{
    RUN_TEST_CASE(LOG_Spy, AfterInitCallCountIsZero);
    RUN_TEST_CASE(LOG_Spy, SpyReturnsCallCount);
    RUN_TEST_CASE(LOG_Spy, AfterInitSpyReturnsEmptyString);
    RUN_TEST_CASE(LOG_Spy, SpyReturnsMessages);
    RUN_TEST_CASE(LOG_Spy, ThrowsFlagWhenMessageTooLong);
    RUN_TEST_CASE(LOG_Spy, LOG_SpyHasCallHistory);
}