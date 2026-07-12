#include "unity_fixture.h"

TEST_GROUP_RUNNER(Logger)
{   
    RUN_TEST_CASE(Logger, SpyDirectCallCatchesArguments);
    RUN_TEST_CASE(Logger, NoInitPrintIsNULL);
    RUN_TEST_CASE(Logger, NoInitNoCrash);
    RUN_TEST_CASE(Logger, NoInitThrowsError);
    RUN_TEST_CASE(Logger, NoInitDoesNotCallSpy);
    RUN_TEST_CASE(Logger, InitSetsPrintingFunction);
    RUN_TEST_CASE(Logger, SpyIndirectCallCatchesArguments);
    RUN_TEST_CASE(Logger, LoggerCantDereferenceItself);
    RUN_TEST_CASE(Logger, NoMoreLogsUntilSygnalingTransmitEnd);
    RUN_TEST_CASE(Logger, SecondLogIsCalledAfterWakeUp);
    RUN_TEST_CASE(Logger, ScheduledLogsIsCalledAfterWakeUp);
    RUN_TEST_CASE(Logger, WakeupWithNoScheduledLogsDoesntCallPrint);
    RUN_TEST_CASE(Logger, MoreThanOneLogCanBeScheduled);
    RUN_TEST_CASE(Logger, AllLogsAreSentCorrectly);
    RUN_TEST_CASE(Logger, AfterInitBufferContentsAreZeroes);
    RUN_TEST_CASE(Logger, LoggerDoesntCurruptMemory);
}