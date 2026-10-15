#include "unity_fixture.h"
#include "QuarkTS.h"
#include "fff.h"

#define TIMER_TICK 1.0f

void fakeTask_callback(qEvent_t e);
qTask_t fakeTask;
static uint32_t tickSet = 0.0f;

void SetTime(uint32_t time)
{
    tickSet = time;
}

uint32_t GetTime()
{
    return tickSet;
}

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(fakeTask_callback, qEvent_t);


TEST_GROUP(QuarkTS);

TEST_SETUP(QuarkTS)
{
    RESET_FAKE(fakeTask_callback);
    FFF_RESET_HISTORY();
    SetTime(0);
    qOS_Setup(GetTime, TIMER_TICK, NULL);
}

TEST_TEAR_DOWN(QuarkTS)
{
}

TEST(QuarkTS,WrongTime_NoRealese)
{
    qOS_Add_Task(&fakeTask,fakeTask_callback,0,100,qPeriodic,qEnabled,NULL);

    SetTime(99);
    qOS_Run();

    TEST_ASSERT_EQUAL(0,fakeTask_callback_fake.call_count);

    qOS_Remove_Task(&fakeTask);
}

TEST(QuarkTS,CorrectTime_Release)
{
    qOS_Add_Task(&fakeTask,fakeTask_callback,0,100,qPeriodic,qEnabled,NULL);

    SetTime(100);
    qOS_Run();

    TEST_ASSERT_EQUAL(1,fakeTask_callback_fake.call_count);

    qOS_Remove_Task(&fakeTask);
}

