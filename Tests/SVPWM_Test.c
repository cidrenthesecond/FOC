#include <unity_fixture.h>
#include "fff.h"
#include "PWM.h"

//UUT
#include "SVPWM.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(PWM_SetPhaseA, uint32_t);
FAKE_VOID_FUNC(PWM_SetPhaseB, uint32_t);
FAKE_VOID_FUNC(PWM_SetPhaseC, uint32_t);

TEST_GROUP(SVPWM);

#define DC_LINK_VOLTAGE 30.0f
#define ARR 65535
static SVPWM_InitStruct init = {ARR, DC_LINK_VOLTAGE};
 

TEST_SETUP(SVPWM)
{
    RESET_FAKE(PWM_SetPhaseA);
    RESET_FAKE(PWM_SetPhaseB);
    RESET_FAKE(PWM_SetPhaseC);
    FFF_RESET_HISTORY();

    SVPWM_Create(init);
}

TEST_TEAR_DOWN(SVPWM)
{
    SVPWM_Destroy();
}

TEST(SVPWM, AfterInitAllPhasesAreSetToZero)
{   
    TEST_ASSERT_EQUAL(1,PWM_SetPhaseA_fake.call_count);
    TEST_ASSERT_EQUAL(1,PWM_SetPhaseB_fake.call_count);
    TEST_ASSERT_EQUAL(1,PWM_SetPhaseC_fake.call_count);
    
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseA_fake.arg0_val);
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseB_fake.arg0_val);
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseC_fake.arg0_val);
}

TEST(SVPWM, AfterInitAllParametersAreSet)
{
    
}


TEST(SVPWM, AfterDestroyAllPhasesAreSetToZero)
{   
    SVPWM_Destroy();

    TEST_ASSERT_EQUAL(2,PWM_SetPhaseA_fake.call_count);
    TEST_ASSERT_EQUAL(2,PWM_SetPhaseB_fake.call_count);
    TEST_ASSERT_EQUAL(2,PWM_SetPhaseC_fake.call_count);
    
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseA_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseB_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(0,PWM_SetPhaseC_fake.arg0_history[1]);
}


IGNORE_TEST(SVPWM, ZeroVoltage)
{
    SVPWM_Proccess(0,0);

    TEST_ASSERT_INT32_WITHIN(2,32767,PWM_SetPhaseA_fake.arg0_history[1]);
    TEST_ASSERT_INT32_WITHIN(2,32767,PWM_SetPhaseB_fake.arg0_history[1]);
    TEST_ASSERT_INT32_WITHIN(2,32767,PWM_SetPhaseC_fake.arg0_history[1]);
}

