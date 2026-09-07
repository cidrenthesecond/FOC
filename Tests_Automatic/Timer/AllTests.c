#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Timer Tests...\n");
    RUN_TEST_GROUP(Timer);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}