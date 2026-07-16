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
    printf("\nLogCircularBuffer tests running...\n");
    RUN_TEST_GROUP(LogCircularBuffer);
    //printf("\nLogger tests running...\n");
    //RUN_TEST_GROUP(Logger);

}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}