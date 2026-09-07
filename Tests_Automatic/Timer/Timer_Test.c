#include "unity_fixture.h"
#include "fff.h"

#include "Timer.h"

void FirstFakeFunction();
void SecondFakeFunction();
void ThirdFakeFunction();

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(FirstFakeFunction);
FAKE_VOID_FUNC(SecondFakeFunction);
FAKE_VOID_FUNC(ThirdFakeFunction);

TEST_GROUP(Timer);

TEST_SETUP(Timer)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(FirstFakeFunction);
    RESET_FAKE(SecondFakeFunction);
    RESET_FAKE(ThirdFakeFunction);

    Timer_Init();
}

TEST_TEAR_DOWN(Timer)
{

}

TEST(Timer, CallingWakeupWithoutInitThrowsError)
{
    Timer_Destroy();
    
    TEST_ASSERT_EQUAL(TIMER_NO_INIT,Timer_Wakeup());
}