#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nLogger tests running...\n");
    RUN_TEST_GROUP(Logger_Test);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}