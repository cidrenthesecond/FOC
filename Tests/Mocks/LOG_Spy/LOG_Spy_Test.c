#include "unity_fixture.h"

#include "LOG_Spy.h"

TEST_GROUP(LOG_Spy);

TEST_SETUP(LOG_Spy)
{
    LOG_Spy_Init();
}

TEST_TEAR_DOWN(LOG_Spy)
{
    
}

TEST(LOG_Spy, AfterInitCallCountIsZero)
{
    LOG("miau");
    LOG_Spy_Init();

    TEST_ASSERT_EQUAL(0,LOG_Spy_GetCallCount());
}

TEST(LOG_Spy, SpyReturnsCallCount)
{
    LOG("statek");
    LOG("oscyloskop");

    TEST_ASSERT_EQUAL(2,LOG_Spy_GetCallCount());
}

TEST(LOG_Spy, AfterInitSpyReturnsEmptyString)
{
    LOG("zasilacz");
    LOG_Spy_Init();

    TEST_ASSERT_EQUAL_STRING("",LOG_Spy_GetMessage());
    TEST_ASSERT_EQUAL(0, LOG_Spy_GetCallCount());
}

TEST(LOG_Spy, SpyReturnsMessages)
{
    LOG("klawiatura");

    TEST_ASSERT_EQUAL_STRING("klawiatura",LOG_Spy_GetMessage());
    TEST_ASSERT_EQUAL(1, LOG_Spy_GetCallCount());
}

IGNORE_TEST(LOG_Spy, ThrowsFlagWhenMessageTooLong)
{

}

IGNORE_TEST(LOG_Spy, LOG_SpyHasCallHistory)
{

}