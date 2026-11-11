#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning PID Tests\n");
    RUN_TEST_GROUP(PID);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}