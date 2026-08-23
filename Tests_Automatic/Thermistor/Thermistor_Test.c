#include "unity_fixture.h"
#include "fff.h"
#include "ADC_Service.h"
#include "Thermistor_LUT.h"

#include "Thermistor.h"

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(uint16_t, GetNtcVoltage);

TEST_GROUP(Thermistor);

TEST_SETUP(Thermistor)
{   
    RESET_FAKE(GetNtcVoltage);
    FFF_RESET_HISTORY();

}

TEST_TEAR_DOWN(Thermistor)
{

}

TEST(Thermistor, FakeAdcReturnsValues)
{
    uint16_t returnSequence = 125;
    SET_RETURN_SEQ(GetNtcVoltage,&returnSequence,1);

    TEST_ASSERT_EQUAL(returnSequence,GetNtcVoltage());
}

TEST(Thermistor, TooHighVoltageThrowsError)
{
    uint16_t fiveVolts = 5000;
    SET_RETURN_SEQ(GetNtcVoltage,&fiveVolts,1);

    TEST_ASSERT_EQUAL(NTC_TOO_HIGH_VOLTAGE,Thermistor_GetHeatsinkTemp());
}

TEST(Thermistor, ZeroVoltageThrowsError)
{
    uint16_t zeroVolts = 0;
    SET_RETURN_SEQ(GetNtcVoltage,&zeroVolts,1);

    TEST_ASSERT_EQUAL(NTC_TOO_LOW_VOLTAGE,Thermistor_GetHeatsinkTemp());
}

TEST(Thermistor, VoltageWithinRangeDoesntThrowError)
{
    uint16_t oneVolt = 1000;
    SET_RETURN_SEQ(GetNtcVoltage,&oneVolt,1);

    int32_t result = Thermistor_GetHeatsinkTemp();

    TEST_ASSERT(result != NTC_TOO_HIGH_VOLTAGE);
    TEST_ASSERT(result != NTC_TOO_LOW_VOLTAGE);
}

TEST(Thermistor, WhenVoltageMatchesPerfectlyReturnsCorrectValue)
{
    for(uint8_t i = 0; i < NTC_LUT_ENTRIES - 1; i++)
    {
        uint16_t voltage = NTC_LUT[i].NTC_Voltage;
        SET_RETURN_SEQ(GetNtcVoltage,&voltage,1);

        TEST_ASSERT_EQUAL(NTC_LUT[i].temperature,Thermistor_GetHeatsinkTemp());
    }

}

TEST(Thermistor, InterpolatesBetweenLutPoints)
{
    uint16_t voltage = 1550;
    SET_RETURN_SEQ(GetNtcVoltage,&voltage,1);

    TEST_ASSERT_INT32_WITHIN(1,468,Thermistor_GetHeatsinkTemp());
}

TEST(Thermistor, VoltageBeyondLutEntriesThrowsError)
{
    uint16_t voltage = NTC_LUT[NTC_LUT_ENTRIES - 1].NTC_Voltage + 10;
    SET_RETURN_SEQ(GetNtcVoltage,&voltage,1);

    TEST_ASSERT_EQUAL(NTC_TOO_HIGH_VOLTAGE, Thermistor_GetHeatsinkTemp());
}