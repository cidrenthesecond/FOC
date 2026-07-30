#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nCMSIS tests running...\n");
    RUN_TEST_GROUP(CMSIS);
    printf("\nFake_DcLink tests running...\n");
    RUN_TEST_GROUP(Fake_DcLink);
    printf("\nRelay tests running...\n");
    RUN_TEST_GROUP(Relay);
    printf("\nPID tests running...\n");
    RUN_TEST_GROUP(PID);
    printf("\nFakeMicroTime tests running...\n");
    RUN_TEST_GROUP(FakeMicroTime);
    printf("\nSVPWM tests running...\n");
    RUN_TEST_GROUP(SVPWM);
    printf("\nFIFO tests running...\n");
    RUN_TEST_GROUP(FIFO_Test);
    printf("\nLogger tests running...\n");
    RUN_TEST_GROUP(Logger_Test);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}