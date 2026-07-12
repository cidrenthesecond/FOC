#include "unity.h"
#include "unity_fixture.h"
#include "unity_internals.h"

TEST_GROUP_RUNNER(PID)
{
    RUN_TEST_CASE(PID, GainsAreSetable);
    RUN_TEST_CASE(PID, SettingGainNegativeDoesntChangeThem);
    RUN_TEST_CASE(PID, PGainTest);
    RUN_TEST_CASE(PID, IGainStepResponse);
    RUN_TEST_CASE(PID, IntegralNoSignalResponse);
    RUN_TEST_CASE(PID, IntegralChangesSigns);
    RUN_TEST_CASE(PID, DerivateConstSignalResponse);
    RUN_TEST_CASE(PID, DerivativeStepSignalResponse);
    RUN_TEST_CASE(PID, CombinedPIDResponseMatchesExpectedSum);
    RUN_TEST_CASE(PID, DerivativeRampsResponse);
    RUN_TEST_CASE(PID, SaturationIsSetButNotUsed);
    RUN_TEST_CASE(PID, OutputExactlyAtSaturationIsNotClamped);
    RUN_TEST_CASE(PID, SaturationWorksBothWays);
    RUN_TEST_CASE(PID, ResetIntegral);
    RUN_TEST_CASE(PID, ResetPreviousError);
    RUN_TEST_CASE(PID, DefaultSaturationIsMinusOne);
    RUN_TEST_CASE(PID, NegativeSaturationDoesntOverwritePreviousValue);
    RUN_TEST_CASE(PID, MultiInstace);
    RUN_TEST_CASE(PID, SaturationCanBeDeactivated);


    RUN_TEST_CASE(PID, NoWindupUpperBound);
    RUN_TEST_CASE(PID, NoWindupLowerBound);
    RUN_TEST_CASE(PID, ResetClearsWindupCorrection);

}