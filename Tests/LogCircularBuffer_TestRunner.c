#include "unity_fixture.h"

TEST_GROUP_RUNNER(LogCircularBuffer)
{
    RUN_TEST_CASE(LogCircularBuffer, BufferAfterInitContainsNothing);
    RUN_TEST_CASE(LogCircularBuffer, GettingFromNoInitBufferThrowsError);
    RUN_TEST_CASE(LogCircularBuffer, PuttingToNoInitBufferThrowError);
    RUN_TEST_CASE(LogCircularBuffer, BufferStoresOneItem);
    RUN_TEST_CASE(LogCircularBuffer, BufferStoresMultipleItems);
    RUN_TEST_CASE(LogCircularBuffer, BufferDoesntCurruptMemory);
    RUN_TEST_CASE(LogCircularBuffer, BufferTruncatesMessages);
    RUN_TEST_CASE(LogCircularBuffer, BufferThrowsErrorWhenGettingFromEmpty);
    RUN_TEST_CASE(LogCircularBuffer, BufferReturnsGivenItemAndItsLength);
    RUN_TEST_CASE(LogCircularBuffer, BufferReturnsSameDataIfNotEndOfProccesed);
    RUN_TEST_CASE(LogCircularBuffer, AdvancingClearsUnusedIndexes);
    RUN_TEST_CASE(LogCircularBuffer, BufferReturnsItemsInCorrectOrder);
    RUN_TEST_CASE(LogCircularBuffer, BufferDoesntReturnFullLogOnEdge);
    RUN_TEST_CASE(LogCircularBuffer, BufferReturnsWrappedLogInTwoTries);
    RUN_TEST_CASE(LogCircularBuffer, PuttingIntoFullBufferReturnsError);
    RUN_TEST_CASE(LogCircularBuffer, TerminatorLandingOnLastByteReturnsFull);
    RUN_TEST_CASE(LogCircularBuffer, PutReturnsSuccessOnNormalWrite);
    RUN_TEST_CASE(LogCircularBuffer, AdvanceGetIndexWithNothingPendingIsSafe);
}