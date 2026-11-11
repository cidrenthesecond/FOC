#include "unity_fixture.h"

TEST_GROUP_RUNNER(FIFO_Test)
{
    RUN_TEST_CASE(FIFO_Test, InitWithZeroLengthReturnsNULL);
    RUN_TEST_CASE(FIFO_Test, CallingDestroyMoreThanOnceIsSafe);
    RUN_TEST_CASE(FIFO_Test, gettingFromEmptyBufferThrowsError);
    RUN_TEST_CASE(FIFO_Test, puttingOrGettingFromNoInitThrowsError);
    RUN_TEST_CASE(FIFO_Test, NullLogThrowsError);
    RUN_TEST_CASE(FIFO_Test, FifoStoresAndReturnsOneLog);
    RUN_TEST_CASE(FIFO_Test, FIFO_Get_ReturnsPushedLogLength);
    RUN_TEST_CASE(FIFO_Test, SuccessfullPutAndGetThrowsInfo);
    RUN_TEST_CASE(FIFO_Test, Put_LogExceedingMaxLength_TruncatesContent);
    RUN_TEST_CASE(FIFO_Test, Get_WhenInputExceedsMaxCapacity_ReceivedLengthEqualsMaxLength);
    RUN_TEST_CASE(FIFO_Test, FifoStoresMultipleLogsAndReturnsThemInCorrectOrder);
    RUN_TEST_CASE(FIFO_Test, Get_FromEmptyBuffer_ReturnsFifoEmpty);
    RUN_TEST_CASE(FIFO_Test, StoresDeclaredNumOfLogs);
    RUN_TEST_CASE(FIFO_Test, BufferOverFlowThrowsError);
    RUN_TEST_CASE(FIFO_Test, Put_WhenBufferOverflows_QueuesOverflowWarningMessage);
    RUN_TEST_CASE(FIFO_Test, Put_WhenBufferIsFull_ReturnsFifoFullAndRejectsData);
    RUN_TEST_CASE(FIFO_Test, Get_AfterRepeatedOverflow_QueuesOverflowWarningMessageAgain);
    RUN_TEST_CASE(FIFO_Test, OverFlowLogCorrectLengthIsReturned);
    RUN_TEST_CASE(FIFO_Test, GetIndexOverFlow);
    RUN_TEST_CASE(FIFO_Test, MultipleInstances_MaintainSeparateBuffers);
    RUN_TEST_CASE(FIFO_Test, QueueSizeCanBeDetermined);
    RUN_TEST_CASE(FIFO_Test, PutGetPutLength);
    
}