#include "unity_fixture.h"

TEST_GROUP_RUNNER(CMSIS)
{
    RUN_TEST_CASE(CMSIS,clarkeAlpha);
    RUN_TEST_CASE(CMSIS,clarkeBeta);
    RUN_TEST_CASE(CMSIS,InverseClarkeAlpha);
    RUN_TEST_CASE(CMSIS,InverseClarkeBeta);
    RUN_TEST_CASE(CMSIS,ClarkeRound);
    RUN_TEST_CASE(CMSIS,ParkeAlpha);
}