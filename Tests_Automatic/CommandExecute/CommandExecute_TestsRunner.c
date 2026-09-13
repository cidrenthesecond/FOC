#include "unity_fixture.h"

TEST_GROUP_RUNNER(CommandExecute)
{
    RUN_TEST_CASE(CommandExecute, Register_WhenNoInit_ThrowsFail);
    RUN_TEST_CASE(CommandExecute, Register_WhenIndexEqualToZero_ThrowsFail);
}