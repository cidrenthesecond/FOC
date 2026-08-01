#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning FakeDcLink tests...\n");
    RUN_TEST_GROUP(Fake_DcLink);
    printf("\nRunning Relay tests...\n");
    RUN_TEST_GROUP(Relay);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}