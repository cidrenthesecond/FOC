#include "unity_fixture.h"

TEST_GROUP_RUNNER(QuarkTS)
{
    RUN_TEST_CASE(QuarkTS,WrongTime_NoRealese);
    RUN_TEST_CASE(QuarkTS,CorrectTime_Release);
}