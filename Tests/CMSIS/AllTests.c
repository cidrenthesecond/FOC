#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nCMSIS tests running...\n");
    RUN_TEST_GROUP(CMSIS);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}