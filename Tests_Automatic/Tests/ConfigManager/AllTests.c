#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Config Manager Tests...\n");
    RUN_TEST_GROUP(ConfigManager);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}