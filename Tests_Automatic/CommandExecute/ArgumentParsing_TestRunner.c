#include "unity_fixture.h"

TEST_GROUP_RUNNER(ArgumentParsing)
{
    RUN_TEST_CASE(ArgumentParsing,OneArgumentIsParsedCorrectly);
    RUN_TEST_CASE(ArgumentParsing,BothArgumentsAreParsedCorrectly);
    RUN_TEST_CASE(ArgumentParsing,MaxArgumentsAreParsedCorrectly);
}