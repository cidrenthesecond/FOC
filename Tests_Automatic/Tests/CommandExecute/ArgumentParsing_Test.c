#include "unity_fixture.h"
#include "string.h"

#include "CommandExecute.h"

static char spy_arguments[MAX_ARGS][10] = {0};
static int  spy_argument_count = 0;

void Spy_Init()
{
    memset(spy_arguments, 0,sizeof(spy_arguments));
    spy_argument_count = 0;
}

void Spy_Command(int argc, char *argv[])
{
    for(int index = 0; index < argc; index++)
    {
        strcpy(spy_arguments[index],argv[index]);
        spy_argument_count++;
    }
}

TEST_GROUP(ArgumentParsing);

TEST_SETUP(ArgumentParsing)
{
    CommandExecute_Init();
    Spy_Init();
    CommandExecute_Register(3,Spy_Command);
}

TEST_TEAR_DOWN(ArgumentParsing)
{
    CommandExecute_Destroy();
}

TEST(ArgumentParsing,OneArgumentIsParsedCorrectly)
{
    CommandExecute("cmd 3 1");
    int firstArgument = strtol(spy_arguments[0],NULL,10);

    TEST_ASSERT_EQUAL(1,spy_argument_count);
    TEST_ASSERT_EQUAL(1,firstArgument);
}

TEST(ArgumentParsing,BothArgumentsAreParsedCorrectly)
{
    CommandExecute("cmd 3 1 2");
    int firstArgument = strtol(spy_arguments[0],NULL,10);
    int secondargument= strtol(spy_arguments[1],NULL,10);

    TEST_ASSERT_EQUAL(2,spy_argument_count);
    TEST_ASSERT_EQUAL(1,firstArgument);
    TEST_ASSERT_EQUAL(2,secondargument);
}

TEST(ArgumentParsing,MaxArgumentsAreParsedCorrectly)
{
    CommandExecute("cmd 3 0 1 2 3 4");
    int argumentsParsed[MAX_ARGS];

    for(uint32_t index = 0; index < MAX_ARGS; index++)
    {
        argumentsParsed[index] = strtol(spy_arguments[index],NULL,10);
        TEST_ASSERT_EQUAL(index,argumentsParsed[index]);
    }
    TEST_ASSERT_EQUAL(MAX_ARGS,spy_argument_count);
}

