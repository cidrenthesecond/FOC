#include "unity_fixture.h"

static void RunAllTests(void)
{   
    printf("\nRunning Modbus Rtu Tests\n");
    RUN_TEST_GROUP(Modbus_Rtu);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}