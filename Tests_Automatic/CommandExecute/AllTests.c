#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Command Execute Tests...\n");
    RUN_TEST_GROUP(InitAndRegister);
    RUN_TEST_GROUP(CommandExecute);
    RUN_TEST_GROUP(ArgumentParsing);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}