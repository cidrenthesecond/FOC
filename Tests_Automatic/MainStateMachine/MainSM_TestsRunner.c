#include "unity_fixture.h"

TEST_GROUP_RUNNER(MainSM)
{
    RUN_TEST_CASE(MainSM,firstRunCausesEnterToInit);
    RUN_TEST_CASE(MainSM,SuccesfullSystemInitEntersInitState);
    RUN_TEST_CASE(MainSM, OnFailedInitEntersEmergencyState);
    RUN_TEST_CASE(MainSM, ErrorInit_ResetError_EntersInit);
    RUN_TEST_CASE(MainSM, ErrorInit_RequestChange_Ignored);
    RUN_TEST_CASE(MainSM, ErrorInit_RequestChange_Ignored);
}