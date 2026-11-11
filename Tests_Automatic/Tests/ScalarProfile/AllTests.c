#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Scalar Profile tests...\n");
    RUN_TEST_GROUP(ScalarProfile);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}