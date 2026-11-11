#include "unity.h"
#include "unity_fixture.h"

#include "arm_math.h"

TEST_GROUP(CMSIS);

TEST_SETUP(CMSIS)
{  
}

TEST_TEAR_DOWN(CMSIS)
{
}

TEST(CMSIS,clarkeAlpha)
{   
    float Ia = 1.0f;
    float Ib = -0.5f;
    //float Ic = -0.5f;
    float I_alpha = -255;
    float I_beta = -255;
    arm_clarke_f32(Ia,Ib,&I_alpha,&I_beta);

    TEST_ASSERT_FLOAT_WITHIN(0.01,1,I_alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01,0,I_beta);
}

TEST(CMSIS,clarkeBeta)
{
    float Ia = 0.0f;
    float Ib = 0.8660254f;
    float I_alpha = -255;
    float I_beta = -255;

    arm_clarke_f32(Ia,Ib,&I_alpha,&I_beta);

    TEST_ASSERT_FLOAT_WITHIN(0.01,0.0f,I_alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01,1.0f,I_beta);
}

TEST(CMSIS,InverseClarkeAlpha)
{
    float I_alpha = 1.0f;
    float I_beta = 0.0f;
    float Ia = -255;
    float Ib = -255;

    arm_inv_clarke_f32(I_alpha,I_beta,&Ia,&Ib);
    TEST_ASSERT_FLOAT_WITHIN(0.01,1.0f,Ia);
    TEST_ASSERT_FLOAT_WITHIN(0.01,-0.5f,Ib);
}

TEST(CMSIS,InverseClarkeBeta)
{
    float I_alpha = 0.0f;
    float I_beta = 1.0f;
    float Ia = -255;
    float Ib = -255;

    arm_inv_clarke_f32(I_alpha,I_beta,&Ia,&Ib);
    TEST_ASSERT_FLOAT_WITHIN(0.01,0.0f,Ia);
    TEST_ASSERT_FLOAT_WITHIN(0.01,0.8660254f,Ib);
}

TEST(CMSIS,ClarkeRound)
{
    float Ia = 1.0f;
    float Ib = -0.5f;

    float I_alpha;
    float I_Beta;
    arm_clarke_f32(Ia,Ib,&I_alpha,&I_Beta);
    
    float Ia_Result;
    float Ib_Result;
    arm_inv_clarke_f32(I_alpha,I_Beta,&Ia_Result,&Ib_Result);

    TEST_ASSERT_FLOAT_WITHIN(0.01,Ia,Ia_Result);
    TEST_ASSERT_FLOAT_WITHIN(0.01,Ib,Ib_Result);
}

TEST(CMSIS,ParkeAlpha)
{
    float I_alpha = 1.0f;
    float I_beta = 0.0f;
    float Id = -255;
    float Iq = -255;
    //theta 0deg
    arm_park_f32(I_alpha,I_beta,&Id,&Iq,0.0f,1.0f);// TO DO DEFINE/ENUM

    TEST_ASSERT_FLOAT_WITHIN(0.01f,1.0f,Id);
    TEST_ASSERT_FLOAT_WITHIN(0.01f,0.0f,Iq);
}