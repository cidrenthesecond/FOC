#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning LOG_Spy Tests\n");
    RUN_TEST_GROUP(LOG_Spy);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}