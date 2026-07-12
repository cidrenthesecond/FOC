#include "unity.h"
#include "unity_fixture.h"

#include "fff.h"
#include "PrintLogSpy.h"
#include "stdlib.h"

#include "Logger.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(PrintLogSpy,const char *,uint8_t);

static char logBuffer[40];


TEST_GROUP(Logger);

TEST_SETUP(Logger)
{
    RESET_FAKE(PrintLogSpy);
    FFF_RESET_HISTORY();

    memset(logBuffer,0xAA,sizeof(logBuffer));

    LOG_Init(PrintLogSpy,logBuffer+10,20);   
}

TEST_TEAR_DOWN(Logger)
{
    LOG_Destroy();
}

TEST(Logger, SpyDirectCallCatchesArguments)
{
    PrintLogSpy("Hello World",11);

    TEST_ASSERT_EQUAL_STRING("Hello World",PrintLogSpy_fake.arg0_val);
    TEST_ASSERT_EQUAL(11,PrintLogSpy_fake.arg1_val);
}

TEST(Logger, NoInitPrintIsNULL)
{   
    LOG_Destroy();

    TEST_ASSERT_EQUAL_PTR(NULL,LOG_GetPrintingFunction());
}

TEST(Logger, NoInitNoCrash)
{   
    LOG_Destroy();

    LOG("Hello World");
}

TEST(Logger, NoInitThrowsError)
{   
    LOG_Destroy();

    TEST_ASSERT_EQUAL(LOGGER_NOINIT,LOG("Hello World"));
}

TEST(Logger, NoInitDoesNotCallSpy)
{
    LOG_Destroy();
    LOG("Hello World");

    TEST_ASSERT_EQUAL(0,PrintLogSpy_fake.call_count);
}

TEST(Logger, InitSetsPrintingFunction)
{
    TEST_ASSERT_EQUAL_PTR(PrintLogSpy,LOG_GetPrintingFunction());
}

TEST(Logger, SpyIndirectCallCatchesArguments)
{
    LOG("Hello World");
    TEST_ASSERT_EQUAL(1,PrintLogSpy_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Hello World",PrintLogSpy_fake.arg0_val);
    TEST_ASSERT_EQUAL(11,PrintLogSpy_fake.arg1_val);
}

TEST(Logger, LoggerCantDereferenceItself)
{
    TEST_ASSERT_EQUAL(LOGGER_NULL_PTR, LOG(NULL));
    TEST_ASSERT_EQUAL(0,PrintLogSpy_fake.call_count);
}

TEST(Logger, NoMoreLogsUntilSygnalingTransmitEnd)
{
    LOG("pizza");
    LOG("kittyMeow");

    TEST_ASSERT_EQUAL(1,PrintLogSpy_fake.call_count);
}

TEST(Logger, SecondLogIsCalledAfterWakeUp)
{
    LOG("cup");
    LOG_WakeUp();
    LOG("mug");

    TEST_ASSERT_EQUAL(2,PrintLogSpy_fake.call_count);
}

TEST(Logger, ScheduledLogsIsCalledAfterWakeUp)
{
    LOG("gamepad");
    LOG("mouse");

    LOG_WakeUp(); //LOG WakeUp Fakes finished transmission interrupt.


    TEST_ASSERT_EQUAL('m',logBuffer[10]);
    TEST_ASSERT_EQUAL(2, PrintLogSpy_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("mouse",PrintLogSpy_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(5,PrintLogSpy_fake.arg1_history[1]);
}

TEST(Logger, WakeupWithNoScheduledLogsDoesntCallPrint)
{
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(0,PrintLogSpy_fake.call_count);
}

TEST(Logger, MoreThanOneLogCanBeScheduled)
{
    LOG("cable");
    LOG("drugs");
    LOG("water");

    LOG_WakeUp();
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(3,PrintLogSpy_fake.call_count);
}

TEST(Logger, AllLogsAreSentCorrectly)
{
    LOG("Chopstick");
    LOG("ketchup");
    LOG("board");

    LOG_WakeUp();
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(3,PrintLogSpy_fake.call_count);

    TEST_ASSERT_EQUAL_STRING("Chopstick",PrintLogSpy_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(strlen("Chopstick"),PrintLogSpy_fake.arg1_history[0]);

    TEST_ASSERT_EQUAL_STRING("ketchup",PrintLogSpy_fake.arg0_history[1]);
    TEST_ASSERT_EQUAL(strlen("ketchup"),PrintLogSpy_fake.arg1_history[1]);

    TEST_ASSERT_EQUAL_STRING("board",PrintLogSpy_fake.arg0_history[2]);
    TEST_ASSERT_EQUAL(strlen("board"),PrintLogSpy_fake.arg1_history[2]);
}

TEST(Logger, AfterInitBufferIsEmpty)
{
    for(int index = 0; index < 20; index++)
        TEST_ASSERT_EQUAL(' ',logBuffer[10+index]);
}

TEST(Logger, LoggerDoesntCurruptMemory)
{
    LOG("123");
    LOG("123456789012345678901234567890");

    TEST_ASSERT_EQUAL_CHAR(0xAA, logBuffer[31]);
}

TEST(Logger, LoggerTruncatesLogs)
{
    LOG("123");
    LOG("123456789012345678901234567890");

    LOG_WakeUp();

    TEST_ASSERT_EQUAL_STRING("1234567890123456789\0",logBuffer + 10);
}














