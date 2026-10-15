#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nQuarkTS tests running...\n");
    RUN_TEST_GROUP(QuarkTS);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}