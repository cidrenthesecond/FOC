#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Thermistor Tests\n");
    RUN_TEST_GROUP(Thermistor);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}