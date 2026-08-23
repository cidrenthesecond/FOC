#include "unity_fixture.h"

TEST_GROUP_RUNNER(Logger_Test)
{
    RUN_TEST_CASE(Logger_Test, FakeCatchesArgumentsAndCallCount);
    RUN_TEST_CASE(Logger_Test,CallingDestroyMoreThanOnceIsSafe);
    RUN_TEST_CASE(Logger_Test, CallingInterfaceWithNoInitThrowsError);
    RUN_TEST_CASE(Logger_Test, InitWithNULLThrowsError);
    RUN_TEST_CASE(Logger_Test, InitWithZeroFifoLengthThrowsError);
    RUN_TEST_CASE(Logger_Test, LoggerUsesPassedFunction);
    RUN_TEST_CASE(Logger_Test, LoggerParsesCorrectTextAndLength);
    RUN_TEST_CASE(Logger_Test, WakeUpWithoutLogsDoesNothing);
    RUN_TEST_CASE(Logger_Test, NewLogWhileBusyIsQueued);
    RUN_TEST_CASE(Logger_Test, LoggerReturnsLogsInCorrectOrder);
    RUN_TEST_CASE(Logger_Test, DoesNotSendNextLogBeforeWakeUp);
    RUN_TEST_CASE(Logger_Test, WhenLogsAreEmptyNoMoreCalls);
    RUN_TEST_CASE(Logger_Test, AfterEmptyingPeripheralCanBeUsedAgain);
}