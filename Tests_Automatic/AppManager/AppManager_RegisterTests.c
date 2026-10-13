#include "unity_fixture.h"
#include "App_Manger.h"
#include "App_State.h"

TEST_GROUP(app_manager_register);

void initStubFun() {}
void runStubFun() {}
void deinitStubFun() {}

TEST_SETUP(app_manager_register)
{
    
}

TEST_TEAR_DOWN(app_manager_register)
{

}

TEST(app_manager_register, registerState_WithTooHighId_ThrowsError)
{
    AppState_t state = {
        .id = APP_STATE_COUNT,
        .init = initStubFun,
        .run = runStubFun,
        .deinit = deinitStubFun
    };

    TEST_ASSERT_FALSE(App_Manager_Register_State(&state));
}

TEST(app_manager_register, registerState_WithNullinit_ThrowsError)
{
    AppState_t state = {
        .id = APP_STATE_RUN,
        .run = runStubFun,
        .deinit = deinitStubFun
    };

    TEST_ASSERT_FALSE(App_Manager_Register_State(&state));
}

TEST(app_manager_register, registerState_WithNullRun_ThrowsError)
{
    AppState_t state = {
        .id = APP_STATE_RUN,
        .init = initStubFun,
        .deinit = deinitStubFun
    };

    TEST_ASSERT_FALSE(App_Manager_Register_State(&state));
}

TEST(app_manager_register, registerState_WithNullini_ThrowsError)
{
    AppState_t state = {
        .id = APP_STATE_RUN,
        .init = initStubFun,
        .run = runStubFun
    };

    TEST_ASSERT_FALSE(App_Manager_Register_State(&state));
}

TEST(app_manager_register, registerState_ProperRegistration_ReturnsSuccess)
{
    AppState_t state = {
        .id = APP_STATE_RUN,
        .init = initStubFun,
        .run = runStubFun,
        .deinit = deinitStubFun
    };

    TEST_ASSERT_TRUE(App_Manager_Register_State(&state));
}
