#include "unity_fixture.h"

TEST_GROUP_RUNNER(InitAndRegister)
{
    RUN_TEST_CASE(InitAndRegister, Register_WhenNoInit_ThrowsFail);
    RUN_TEST_CASE(InitAndRegister, Register_WhenIndexEqualToZero_ThrowsFail);
    RUN_TEST_CASE(InitAndRegister, Register_WhenIndexIsHigherThanMaxCommands_ThrowsFail);
    RUN_TEST_CASE(InitAndRegister, Register_WhenIndexBiggerThanMaxCommands_ThrowsFail);
    RUN_TEST_CASE(InitAndRegister, Register_WhenCommandIsNull_ThrowsError);
    RUN_TEST_CASE(InitAndRegister, Register_CorrectRegistration_ThrowsSuccess);
    RUN_TEST_CASE(InitAndRegister, Register_TakenSpot_ThrowsError);
    RUN_TEST_CASE(InitAndRegister, Destroy_ClearsCommandsList);
}