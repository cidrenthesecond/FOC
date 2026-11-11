#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning FIFO Tests\n");
    RUN_TEST_GROUP(FIFO_Test);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}