#include "PID.h"
#include "Unity.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(PID);

static PID* UUT;

TEST_SETUP(PID)
{
    UUT = PID_Create();
    PID_Init(UUT);
}

TEST_TEAR_DOWN(PID)
{
    PID_Destroy(UUT);
}

static float StepResponse(PID *obj,int dt,float step)
{
    float result = 0.0f;
    for(int i = 0; i < dt; i++)
    {
        result = PID_Compute(obj,step);
    }
    return result;
}

TEST(PID, GainsAreSetable)
{
    PID_SetKp(UUT,2.0);
    PID_SetKd(UUT,1.5);
    PID_SetKi(UUT,12.25);

    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.0, PID_GetKp(UUT));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.5, PID_GetKd(UUT));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 12.25, PID_GetKi(UUT));
}

TEST(PID, SettingGainNegativeDoesntChangeThem)
{
    PID_SetKp(UUT,2.0);
    PID_SetKd(UUT,1.5);
    PID_SetKi(UUT,12.25);

    PID_SetKp(UUT,-2.0);
    PID_SetKd(UUT,-1.5);
    PID_SetKi(UUT,-12.25);

    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.0, PID_GetKp(UUT));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.5, PID_GetKd(UUT));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 12.25, PID_GetKi(UUT));
}

TEST(PID, PGainTest)
{   
    for(float Kp = 0; Kp < 10; Kp += 0.25)
    {
        PID_SetKp(UUT,Kp);

        for(int i = 0; i < 100; i++)
        {
            float result = PID_Compute(UUT,i);
            TEST_ASSERT_FLOAT_WITHIN(0.1, i*Kp, result);
        }
    }
}

TEST(PID, IGainStepResponse)
{
    float Step = 2.5f;
    float result = 0.0f;

    PID_SetKp(UUT,0.0f);
    PID_SetKi(UUT,1.0f);

    result = StepResponse(UUT,30, Step);

    TEST_ASSERT_TRUE(result > 7.0f);
}

TEST(PID, IntegralNoSignalResponse)
{
    float result = 0.0f;

    PID_SetKp(UUT,0.0f);
    PID_SetKi(UUT,1.0f);

    result = StepResponse(UUT,30, 0.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0f, result);
}

TEST(PID, IntegralChangesSigns)
{
    float result = 0.0f;

    PID_SetKp(UUT,0.0f);
    PID_SetKi(UUT,1.0f);

    result = StepResponse(UUT,10, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0f, result);

    result = StepResponse(UUT,20, -1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, -1.0f, result);
}

TEST(PID, DerivateConstSignalResponse)
{
    PID_SetKp(UUT,0.0f);
    PID_SetKd(UUT,1.0f);

    float result = StepResponse(UUT,10, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0f, result);
}

TEST(PID, DerivativeStepSignalResponse)
{
    PID_SetKp(UUT,0.0f);
    PID_SetKd(UUT,1.0f);

    (void)PID_Compute(UUT,0);
    float result = PID_Compute(UUT,1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 10.0f, result);

    result = StepResponse(UUT,10, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f,result);
}

TEST(PID, DerivativeRampsResponse)
{
    PID_SetKp(UUT,0.0f);
    PID_SetKd(UUT,1.0f);
    // float result_up;
    // float result_down;

    for(int i = 1; i < 10; i++)
    {
        float result_up = PID_Compute(UUT,(float)i);
        float result_down = PID_Compute(UUT,(float)-i);

        TEST_ASSERT_TRUE(result_up > 0); //warning for Tustin
        TEST_ASSERT_TRUE(result_down < 0);
    }
}

TEST(PID, CombinedPIDResponseMatchesExpectedSum)
{
    PID_SetKp(UUT,2.0f);
    PID_SetKi(UUT,1.0f);
    PID_SetKd(UUT,0.5f);

    (void)PID_Compute(UUT,0.0f); // seed previousError = 0

    // error = 1.0, Ts = 0.1 (default)
    // P = 2.0, I = 1*1*0.1 = 0.1, D = 0.5*(1-0)/0.1 = 5.0
    float result = PID_Compute(UUT,1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 7.1f, result);
}

TEST(PID, SaturationIsSetButNotUsed)
{
    float saturation = 10.0f;
    float Kp = 2.0;
    PID_SetKp(UUT,Kp);
    PID_SetSaturation(UUT,saturation);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,4.0,PID_Compute(UUT,2.0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f,-4.0,PID_Compute(UUT,-2.0));
}

TEST(PID, OutputExactlyAtSaturationIsNotClamped)
{
    float saturation = 10.0f;
    PID_SetKp(UUT,10.0f);
    PID_SetSaturation(UUT,saturation);

    float result = PID_Compute(UUT,1.0f); // P term = exactly 10.0
    TEST_ASSERT_FLOAT_WITHIN(0.001f, saturation, result);
}

TEST(PID, SaturationWorksBothWays)
{
    float saturation = 10.0f;
    float error = 1.0f;
    PID_SetKp(UUT,100);
    PID_SetSaturation(UUT,saturation);

    TEST_ASSERT_FLOAT_WITHIN(0.01f,saturation,PID_Compute(UUT,error));
    TEST_ASSERT_FLOAT_WITHIN(0.01f,-saturation,PID_Compute(UUT,-error));
}

TEST(PID, MultiInstace)
{
    PID *TestObject = PID_Create();

    PID_SetKp(TestObject,10.0f);

    TEST_ASSERT_FALSE(PID_GetKp(TestObject) == PID_GetKp(UUT));
    TEST_ASSERT_FALSE(PID_Compute(TestObject,2) == PID_Compute(UUT,2));
}

TEST(PID, ResetIntegral)
{
    PID_SetKi(UUT,1.0f);
    PID_SetKp(UUT,0.0f);
    StepResponse(UUT,10, 5.0f); // build up integral + previousError

    PID_Reset(UUT);
    // if state wasn't reset, this would reflect leftover integral
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, PID_Compute(UUT,0.0f));
}

TEST(PID, ResetPreviousError)
{
    PID_SetKp(UUT,0.0f);
    PID_SetKd(UUT,2.0f);
    StepResponse(UUT,10,5.0f);

    PID_Reset(UUT);
    TEST_ASSERT(PID_Compute(UUT,5.0f) != 0 );
}

TEST(PID, DefaultSaturationIsMinusOne)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f,-1.0f,PID_GetSaturation(UUT));
}

TEST(PID, NegativeSaturationDoesntOverwritePreviousValue)
{
    PID_SetSaturation(UUT,5.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,5.0f,PID_GetSaturation(UUT));

    PID_SetSaturation(UUT,-5.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,5.0f,PID_GetSaturation(UUT));
}

TEST(PID, NoWindupUpperBound)
{   
    float saturation = 2.0f;
    PID_SetKp(UUT,2.0f);
    PID_SetKi(UUT,1.0f);
    PID_SetSaturation(UUT,saturation);
    StepResponse(UUT,500,5.0f);
    PID_SetKp(UUT,0.0f);

    TEST_ASSERT(PID_Compute(UUT,-1.0) < saturation);
}

TEST(PID, NoWindupLowerBound)
{
    float saturation = 2.0f;
    PID_SetKp(UUT,2.0f);
    PID_SetKi(UUT,10.0f);
    PID_SetSaturation(UUT,saturation);
    StepResponse(UUT,500,-5.0f);
    
    PID_SetKp(UUT,0.0f);
    TEST_ASSERT(PID_Compute(UUT,1.0) > -saturation);
}

TEST(PID, SaturationCanBeDeactivated)
{
    PID_SetSaturation(UUT,0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,0.5f,PID_Compute(UUT,1.0f));

    PID_DeactivateSaturation(UUT);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,1.0f,PID_Compute(UUT,1.0f));
}

TEST(PID, ResetClearsWindupCorrection)
{
    float saturation = 1.0f;
    PID_SetKi(UUT,10.0f);
    PID_SetSaturation(UUT,saturation);
    StepResponse(UUT,500,1.0);
    PID_DeactivateSaturation(UUT);
    PID_Reset(UUT);
    StepResponse(UUT,500,1.0);

    TEST_ASSERT(PID_Compute(UUT,1.0) > 1.0);
}