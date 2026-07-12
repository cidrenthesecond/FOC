#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(Fake_DcLink)
{
    RUN_TEST_CASE(Fake_DcLink, NothingIsSetVoltageIsZero);
    RUN_TEST_CASE(Fake_DcLink, AfterSetSameValueIsReturned);
    RUN_TEST_CASE(Fake_DcLink, ResetAfterSetReturnZero);
}