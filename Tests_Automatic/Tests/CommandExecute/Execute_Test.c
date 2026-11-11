#include "unity_fixture.h"
#include "fff.h"

#include "CommandExecute.h"

static void FakeCommand(int argc, char *argv[]);

FAKE_VOID_FUNC(FakeCommand, int, char **);

TEST_GROUP(CommandExecute);

TEST_SETUP(CommandExecute)
{
    RESET_FAKE(FakeCommand);
    FFF_RESET_HISTORY();

    CommandExecute_Init();
    CommandExecute_Register(3,FakeCommand);
}

TEST_TEAR_DOWN(CommandExecute)
{
    CommandExecute_Destroy();
}

TEST(CommandExecute, Execute_Correct)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 3"));
    TEST_ASSERT_EQUAL(1,FakeCommand_fake.call_count);
}

//_______Argument_Count_________

TEST(CommandExecute, Execute_WhenNoArguments_ArgumentsCountIsZero)
{
    CommandExecute("cmd 3");
    TEST_ASSERT_EQUAL(0,FakeCommand_fake.arg0_val);
}

TEST(CommandExecute, Ececute_WhenOneArgument_OneIsParsed)
{
    CommandExecute("cmd 3 1");
    TEST_ASSERT_EQUAL(1,FakeCommand_fake.arg0_val);
}

TEST(CommandExecute, Execute_WhenToManyArguments_MaxArgumentsAreParsed)
{
    CommandExecute("cmd 3 1 2 3 4 5");
    TEST_ASSERT_EQUAL(5,FakeCommand_fake.arg0_val);
}


//________Index_Behavior_________

TEST(CommandExecute, Execute_WhenNoCommandIndex_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd"));
}

TEST(CommandExecute, Execute_WhenIndexEqualToZero_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd 0"));
}

TEST(CommandExecute, Execute_WhenIndexIsHigherThanMaxIndex_ThrowsError)
{
    char command[60];
    sprintf(command,"cmd %u",MAX_COMMANDS+1);

    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute(command));
}

TEST(CommandExecute, Execute_WhenIndexIsNegative_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd -3"));
}

TEST(CommandExecute, Execute_WhenIndexIsLetters_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd abd"));
}

TEST(CommandExecute, Execute_WhenIndexContainsLetters_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd 3as"));
}

TEST(CommandExecute, Execute_WhenIndexContainsPlus_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd +3"));
}

TEST(CommandExecute, Execute_WhenIndexIsLeadByZero_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 03"));
}

//__________Syntax____________

TEST(CommandExecute, Execute_WhenInputStringIsEmpty_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute(""));
}

TEST(CommandExecute, Execute_WhenCMDkeywordIsUppercase_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("CMD 3 2"));
}

TEST(CommandExecute, Execute_WhenImproperKeyword_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("abc 3 5"));
}

TEST(CommandExecute, Execute_WhenKeywordAndIndexAreNotSeparated_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd315"));
}

TEST(CommandExecute, Execute_WhenMultipleSpacesAreUsed_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd      3"));
}

TEST(CommandExecute, Execute_WhenLeadingSpacesAreUsed_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("     cmd 3"));
}

TEST(CommandExecute, Execute_WhenTrailingSpacesAreUsed_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 3     "));
}

TEST(CommandExecute, Execute_WhenInputEndsWithLF_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 3\n"));
}

TEST(CommandExecute, Execute_WhenInputEndsWithCR_ThrowsSuccess)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 3\r"));
}

TEST(CommandExecute, Execute_WhenInputEndsWithCRLF)
{
    TEST_ASSERT_EQUAL(CMD_SUCCESS,CommandExecute("cmd 3\r\n"));
}

TEST(CommandExecute, Execute_WhenInputIsNULL_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute(NULL));
}

//Overall behavior

TEST(CommandExecute, Execute_WhenCommandIsNotRegistered_ThrowsError)
{
    TEST_ASSERT_EQUAL(CMD_FAIL,CommandExecute("cmd 16"));
}

TEST(CommandExecute, Execute_MultipleExecutions)
{
    CommandExecute("cmd 3");
    CommandExecute("cmd 3");
    CommandExecute("cmd 3");

    TEST_ASSERT_EQUAL(3,FakeCommand_fake.call_count);
}

//Boundary conditions
TEST(CommandExecute, Execute_AllIndexesWork)
{
    for(uint32_t i = 1; i <= MAX_COMMANDS; i++)
    {
        CommandExecute_Register(i,FakeCommand);
        char command[60] = {0};
        sprintf(command,"cmd %u",i);
        CommandExecute(command);
    }
    
    TEST_ASSERT_EQUAL(MAX_COMMANDS,FakeCommand_fake.call_count);
}




