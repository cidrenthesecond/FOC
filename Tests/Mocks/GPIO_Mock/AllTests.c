#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning GPIO_Mock Tests\n");
    RUN_TEST_GROUP(GPIO_MOCK);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}