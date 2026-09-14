#include "unity_fixture.h"
#include "fff.h"

#include "CommandExecute.h"

static void FakeCommand1(int argc, char *argv[]);

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(FakeCommand1, int, char **);

TEST_GROUP(InitAndRegister);

static void FILL_COMMAND_LIST()
{
    for(int index = 1; index <= MAX_COMMANDS; index++)
        CommandExecute_Register(index,FakeCommand1);
}

TEST_SETUP(InitAndRegister)
{
    CommandExecute_Init();
    RESET_FAKE(FakeCommand1);
    FFF_RESET_HISTORY();
}

TEST_TEAR_DOWN(InitAndRegister)
{
    CommandExecute_Destroy();
}

TEST(InitAndRegister, Register_WhenNoInit_ThrowsFail)
{
    CommandExecute_Destroy();
    TEST_ASSERT_EQUAL(CMD_FAIL, CommandExecute_Register(1,FakeCommand1));
}

TEST(InitAndRegister, Register_WhenIndexEqualToZero_ThrowsFail)
{
    TEST_ASSERT_EQUAL(CMD_FAIL, CommandExecute_Register(0,FakeCommand1));
}

//ZMIANA NAZWY
TEST(InitAndRegister, Register_WhenIndexIsEqualMaxCommands_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS, CommandExecute_Register(MAX_COMMANDS,FakeCommand1));
}

TEST(InitAndRegister, Register_WhenIndexBiggerThanMaxCommands_ThrowsFail)
{
    TEST_ASSERT_EQUAL(CMD_FAIL, CommandExecute_Register(MAX_COMMANDS + 1,FakeCommand1));
}

TEST(InitAndRegister, Register_WhenCommandIsNull_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute_Register(15,NULL));
}

TEST(InitAndRegister, Register_CorrectRegistration_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute_Register(13,FakeCommand1));
}

TEST(InitAndRegister, Register_TakenSpot_ThrowsError)
{
    CommandExecute_Register(10,FakeCommand1);
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute_Register(10,FakeCommand1));
}

TEST(InitAndRegister, Destroy_ClearsCommandsList)
{
    FILL_COMMAND_LIST();
    CommandExecute_Destroy();
    CommandExecute_Init();

    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute_Register(3,FakeCommand1));
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute_Register(1,FakeCommand1));
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute_Register(20,FakeCommand1));
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute_Register(5,FakeCommand1));
}

