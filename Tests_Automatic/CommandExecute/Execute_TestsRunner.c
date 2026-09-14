#include "unity_fixture.h"
   
TEST_GROUP_RUNNER(CommandExecute)
{
    RUN_TEST_CASE(CommandExecute, Execute_Correct);

    RUN_TEST_CASE(CommandExecute, Execute_WhenNoArguments_ArgumentsCountIsZero);
    RUN_TEST_CASE(CommandExecute, Ececute_WhenOneArgument_OneIsParsed);
    RUN_TEST_CASE(CommandExecute, Execute_WhenToManyArguments_MaxArgumentsAreParsed);

    RUN_TEST_CASE(CommandExecute, Execute_WhenNoCommandIndex_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexEqualToZero_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexIsHigherThanMaxIndex_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexIsNegative_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexIsLetters_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexContainsLetters_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexContainsPlus_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenIndexIsLeadByZero_ThrowsSuccess);

    RUN_TEST_CASE(CommandExecute, Execute_WhenInputStringIsEmpty_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenCMDkeywordIsUppercase_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenImproperKeyword_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenKeywordAndIndexAreNotSeparated_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_WhenMultipleSpacesAreUsed_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenLeadingSpacesAreUsed_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenTrailingSpacesAreUsed_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenInputEndsWithLF_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenInputEndsWithCR_ThrowsSuccess);
    RUN_TEST_CASE(CommandExecute, Execute_WhenInputEndsWithCRLF);
    RUN_TEST_CASE(CommandExecute, Execute_WhenInputIsNULL_ThrowsError);


    RUN_TEST_CASE(CommandExecute, Execute_WhenCommandIsNotRegistered_ThrowsError);
    RUN_TEST_CASE(CommandExecute, Execute_MultipleExecutions);

    RUN_TEST_CASE(CommandExecute, Execute_AllIndexesWork);
}
