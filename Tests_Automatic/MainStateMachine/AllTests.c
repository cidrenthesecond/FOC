#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nMainSM tests running...\n");
    RUN_TEST_GROUP(MainSM);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}