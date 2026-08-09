#include "unity_fixture.h"
#include "fff.h"
#include "PrintLogSpy.h"

#include "Logger.h"
#include "FIFO.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(PrintLogSpy, const char *,uint8_t);

TEST_GROUP(Logger_Test);

#define QUEUE_SIZE 5

FIFO fifo;

TEST_SETUP(Logger_Test)
{
    RESET_FAKE(PrintLogSpy);
    FFF_RESET_HISTORY();

    fifo = FIFO_Create(QUEUE_SIZE);

    LOG_Init(PrintLogSpy, fifo);
}

TEST_TEAR_DOWN(Logger_Test)
{
    LOG_Destroy();
    FIFO_Destroy(fifo);
}

TEST(Logger_Test,FakeCatchesArgumentsAndCallCount)
{
    PrintLogSpy("miau",5);
    TEST_ASSERT_EQUAL(1,PrintLogSpy_fake.call_count);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("miau\0",PrintLogSpy_fake.arg0_history[0],5);
    TEST_ASSERT_EQUAL(5,PrintLogSpy_fake.arg1_history[0]);
}

TEST(Logger_Test,CallingInterfaceWithNoInitThrowsError)
{
    LOG_Destroy();
    TEST_ASSERT_EQUAL(LOGGER_NOINIT,LOG("laptop"));
    TEST_ASSERT_EQUAL(LOGGER_NOINIT,LOG_WakeUp());
}

TEST(Logger_Test,InitWithNULLThrowsError)
{
    LOG_Destroy();
    TEST_ASSERT_EQUAL(LOGGER_FAIL,LOG_Init(NULL,fifo));
}

TEST(Logger_Test,LoggerUsesPassedFunction)
{
    LOG("Mountain");
    TEST_ASSERT_EQUAL(1,PrintLogSpy_fake.call_count);
}

TEST(Logger_Test,LoggerParsesCorrectTextAndLength)
{
    LOG("helmet");
    TEST_ASSERT_EQUAL_CHAR_ARRAY("helmet\n",PrintLogSpy_fake.arg0_val,strlen("helmet") + 1);
    TEST_ASSERT_EQUAL(strlen("helmet")+1,PrintLogSpy_fake.arg1_val);
}

TEST(Logger_Test, WakeUpWithoutLogsDoesNothing)
{
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(0, PrintLogSpy_fake.call_count);
}

TEST(Logger_Test,NewLogWhileBusyIsQueued)
{
    LOG("treeline");
    LOG("house");

    TEST_ASSERT_EQUAL(1,PrintLogSpy_fake.call_count);
}

TEST(Logger_Test, LoggerReturnsLogsInCorrectOrder)
{
    LOG("railway");
    LOG("light");
    LOG("wall");
    LOG_WakeUp();
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(3,PrintLogSpy_fake.call_count);

    TEST_ASSERT_EQUAL_CHAR_ARRAY("light\n", PrintLogSpy_fake.arg0_history[1],strlen("light") + 1);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("wall\n",  PrintLogSpy_fake.arg0_history[2],strlen("wall")+1);
    TEST_ASSERT_EQUAL(strlen("light") + 1,PrintLogSpy_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(strlen("wall") + 1, PrintLogSpy_fake.arg1_history[2]);
}

TEST(Logger_Test, DoesNotSendNextLogBeforeWakeUp)
{
    LOG("first");
    LOG("second");

    TEST_ASSERT_EQUAL(1, PrintLogSpy_fake.call_count);

    LOG_WakeUp();

    TEST_ASSERT_EQUAL(2, PrintLogSpy_fake.call_count);
}

TEST(Logger_Test, WhenLogsAreEmptyNoMoreCalls)
{
    LOG("chair");
    LOG("rocks");
    LOG_WakeUp();
    LOG_WakeUp();
    LOG_WakeUp();

    TEST_ASSERT_EQUAL(2,PrintLogSpy_fake.call_count);
}

TEST(Logger_Test, AfterEmptyingPeripheralCanBeUsedAgain)
{
    LOG("Cat");
    LOG("Dog");
    LOG_WakeUp();
    LOG_WakeUp();
    LOG("Penguin");

    TEST_ASSERT_EQUAL(3,PrintLogSpy_fake.call_count);
}

TEST(Logger_Test, PassedFifoCantBeNULL)
{
    LOG_Destroy();
    TEST_ASSERT_EQUAL(LOGGER_FAIL, LOG_Init(PrintLogSpy,NULL));
}


