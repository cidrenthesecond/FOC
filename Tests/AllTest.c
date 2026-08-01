#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nCMSIS tests running...\n");
    RUN_TEST_GROUP(CMSIS);
    printf("\nFakeMicroTime tests running...\n");
    RUN_TEST_GROUP(FakeMicroTime);
    printf("\nSVPWM tests running...\n");
    RUN_TEST_GROUP(SVPWM);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}