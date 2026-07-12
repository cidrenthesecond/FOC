#include "unity.h"
#include "unity_fixture.h"
#include "FakeDcLink.h"

TEST_GROUP(Fake_DcLink);

TEST_SETUP(Fake_DcLink)
{
    Fake_ResetDcLinkVoltage();
}

TEST_TEAR_DOWN(Fake_DcLink)
{

}

TEST(Fake_DcLink, NothingIsSetVoltageIsZero)
{
    TEST_ASSERT_EQUAL(0, GetDcLinkVoltage());
}

TEST(Fake_DcLink, AfterSetSameValueIsReturned)
{
    Fake_SetDcLinkVoltage(230);
    TEST_ASSERT_EQUAL(230, GetDcLinkVoltage());
}

TEST(Fake_DcLink, ResetAfterSetReturnZero)
{
    Fake_ResetDcLinkVoltage(230);
    Fake_ResetDcLinkVoltage();
    TEST_ASSERT_EQUAL(0, GetDcLinkVoltage());
}



