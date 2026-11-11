#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Sys tick Dispatcher Tests...\n");
    RUN_TEST_GROUP(SysTickDispatcher);
}     
     
int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}