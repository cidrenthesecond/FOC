#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning App_Manger Tests...\n");
    RUN_TEST_GROUP(app_manager);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}