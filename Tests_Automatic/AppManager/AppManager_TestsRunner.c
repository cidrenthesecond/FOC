#include "unity_fixture.h"

TEST_GROUP_RUNNER(app_manager)
{
    RUN_TEST_CASE(app_manager, registerState_WithTooHighId_ThrowsError);
    RUN_TEST_CASE(app_manager, registerState_WithNullinit_ThrowsError);
    RUN_TEST_CASE(app_manager, registerState_WithNullRun_ThrowsError);
    RUN_TEST_CASE(app_manager, registerState_WithNullini_ThrowsError);
}