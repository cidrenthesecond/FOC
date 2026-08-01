#include "unity_fixture.h"

TEST_GROUP_RUNNER(FIFO_Test)
{
    RUN_TEST_CASE(FIFO_Test, gettingFromEmptyBufferThrowsError);
    RUN_TEST_CASE(FIFO_Test, puttingOrGettingFromNoInitThrowsError);
    RUN_TEST_CASE(FIFO_Test, NullLogThrowsError);
    RUN_TEST_CASE(FIFO_Test, FifoStoresAndReturnsOneLog);
    RUN_TEST_CASE(FIFO_Test, SuccessfullPutAndGetThrowsInfo);
    RUN_TEST_CASE(FIFO_Test, WontStoreMoreThanLogSize);
    RUN_TEST_CASE(FIFO_Test, FifoStoresMultipleLogsAndReturnsThemInCorrectOrder);
    RUN_TEST_CASE(FIFO_Test, PutAndGetEmptyThrow);
    RUN_TEST_CASE(FIFO_Test, StoresDeclaredNumOfLogs);
    RUN_TEST_CASE(FIFO_Test, BufferOverFlowThrowsError);
    RUN_TEST_CASE(FIFO_Test, AfterOverFlowSpecialLogIsQueued);
    RUN_TEST_CASE(FIFO_Test, GetIndexOverFlow);
    RUN_TEST_CASE(FIFO_Test, MultiInstaceBehavior);
    RUN_TEST_CASE(FIFO_Test, QueueSizeCanBeDetermined);
    RUN_TEST_CASE(FIFO_Test, TextInSizeBoundsReturnsCorrectLen);
    RUN_TEST_CASE(FIFO_Test, TextLenIsNeverGreaterThanMaxSize);
}