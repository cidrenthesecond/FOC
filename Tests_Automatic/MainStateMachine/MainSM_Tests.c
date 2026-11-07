#include "unity_fixture.h"
#include "MainSM.h"

#include "fff.h"
#include "System_Config.h"
#include "Timer_Driver.h"
#include "NTC_Task.h"
#include "Control_Task.h"
#include "Logger.h"
#include "qkernel.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(NTC_SetActive, bool);
FAKE_VOID_FUNC(Control_Task_SetActive, bool);
FAKE_VOID_FUNC(Control_Before_Launch);
FAKE_VOID_FUNC(PWM_Stop);
FAKE_VOID_FUNC(PWM_Start);
FAKE_VALUE_FUNC(int, LOG, const char*);
FAKE_VALUE_FUNC(SystemInitStatus_t, System_Init);

static void EnterFailedInit();

static uint32_t fakeSystick = 0;
static uint32_t increment   = 0;
void FakeSysTick_Init(uint32_t setIncrement)
{
    fakeSystick = 0;
    increment   = setIncrement;
}
void FakeSysTick_Increment()
{
    fakeSystick += increment;
}
uint32_t FakeSysTick_GetTick()
{
    return fakeSystick;
}


TEST_GROUP(MainSM);

TEST_SETUP(MainSM)
{
    FakeSysTick_Init(2);

    RESET_FAKE(NTC_SetActive);
    RESET_FAKE(Control_Task_SetActive);
    RESET_FAKE(Control_Before_Launch);
    RESET_FAKE(PWM_Stop);
    RESET_FAKE(PWM_Start);
    RESET_FAKE(LOG);
    RESET_FAKE(System_Init);
    FFF_RESET_HISTORY();

    qOS_Setup(FakeSysTick_GetTick, NULL);
}

TEST_TEAR_DOWN(MainSM)
{
}

TEST(MainSM,firstRunCausesEnterToInit)
{
    Enter_MainStateMachine();
    FakeSysTick_Increment();
    qOS_Run();

    TEST_ASSERT_EQUAL_STRING("Entered INIT state",LOG_fake.arg0_val);
}

TEST(MainSM,SuccesfullSystemInitEntersInitState)
{
    Enter_MainStateMachine();
    FakeSysTick_Increment();
    qOS_Run();                          
    TEST_ASSERT_EQUAL_STRING("Entered INIT state",LOG_fake.arg0_val); //INIT ENTRY
    
    SystemInitStatus_t fakeStatus = INIT_SUCCESS;
    SET_RETURN_SEQ(System_Init,&fakeStatus,1);
    FakeSysTick_Increment();
    qOS_Run();                          //Execution of Init

    FakeSysTick_Increment();
    qOS_Run();

    TEST_ASSERT_EQUAL_STRING("Entered IDLE State",LOG_fake.arg0_val);
}

TEST(MainSM, OnFailedInitEntersEmergencyState)
{
    Enter_MainStateMachine();
    FakeSysTick_Increment();
    qOS_Run();                          
    TEST_ASSERT_EQUAL_STRING("Entered INIT state",LOG_fake.arg0_val); //INIT ENTRY
    
    SystemInitStatus_t fakeStatus = INIT_FAIL;
    SET_RETURN_SEQ(System_Init,&fakeStatus,1);
    FakeSysTick_Increment();
    qOS_Run();                                      //Execution of Init

    FakeSysTick_Increment();
    qOS_Run();

    TEST_ASSERT_EQUAL_STRING("Entered ERROR",LOG_fake.arg0_val);
}

TEST(MainSM, ErrorInit_ResetError_EntersInit)
{
    EnterFailedInit();

    Main_RequestErrorReset();
    FakeSysTick_Increment();
    qOS_Run();                              //Init Entry

    TEST_ASSERT_EQUAL_STRING("Entered INIT state",LOG_fake.arg0_val);
}

TEST(MainSM, ErrorInit_RequestChange_Ignored)
{
    EnterFailedInit();

    Main_RequestIDLE();
    FakeSysTick_Increment();
    qOS_Run();                          //IDLE request ignored

    TEST_ASSERT_EQUAL_STRING("Entered ERROR",LOG_fake.arg0_val);

    Main_RequestRUN();
    FakeSysTick_Increment();
    qOS_Run();                            //RUN request ignored

    TEST_ASSERT_EQUAL_STRING("Entered ERROR",LOG_fake.arg0_val);
}

static void EnterFailedInit()
{
    Enter_MainStateMachine();
    FakeSysTick_Increment();
    qOS_Run();                          
    TEST_ASSERT_EQUAL_STRING("Entered INIT state",LOG_fake.arg0_val); //INIT ENTRY
    
    SystemInitStatus_t fakeStatus = INIT_FAIL;
    SET_RETURN_SEQ(System_Init,&fakeStatus,1);
    FakeSysTick_Increment();
    qOS_Run();                                      //Execution of Init

    FakeSysTick_Increment();
    qOS_Run();

    TEST_ASSERT_EQUAL_STRING("Entered ERROR",LOG_fake.arg0_val);
}

static void EnterIdle()
{
    
}


