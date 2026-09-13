#include "unity_fixture.h"
#include "fff.h"

#include "CommandExecute.h"

void FakeCommand(int argc, char *argv[]);

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(FakeCommand, int, char **);

TEST_GROUP(CommandExecute);

TEST_SETUP(CommandExecute)
{
    CommandExecute_Init();
}

TEST_TEAR_DOWN(CommandExecute)
{
    CommandExecute_Destroy();
}

TEST(CommandExecute, Register_WhenNoInit_ThrowsFail)
{
    CommandExecute_Destroy();
    TEST_ASSERT_EQUAL(CMD_FAIL, CommandExecute_Register(1,FakeCommand));
}

TEST(CommandExecute, Register_WhenIndexEqualToZero_ThrowsFail)
{
    TEST_ASSERT_EQUAL(CMD_FAIL, CommandExecute_Register(0,FakeCommand));
}