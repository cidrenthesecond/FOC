#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning WaveGen Tests\n");
    RUN_TEST_GROUP(WaveGen);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}