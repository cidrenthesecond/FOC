#include "unity_fixture.h"

TEST_GROUP_RUNNER(Thermistor)
{
    RUN_TEST_CASE(Thermistor, FakeAdcReturnsValues);
    RUN_TEST_CASE(Thermistor, TooHighVoltageThrowsError);
    RUN_TEST_CASE(Thermistor, ZeroVoltageThrowsError);
    RUN_TEST_CASE(Thermistor, VoltageWithinRangeDoesntThrowError);
    RUN_TEST_CASE(Thermistor, WhenVoltageMatchesPerfectlyReturnsCorrectValue);
    RUN_TEST_CASE(Thermistor, InterpolatesBetweenLutPoints);
    RUN_TEST_CASE(Thermistor, VoltageBeyondLutEntriesThrowsError);
}