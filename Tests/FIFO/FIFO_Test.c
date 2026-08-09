#include "unity_fixture.h"
#include "unity.h"
#include "string.h"

#include "FIFO.h"

TEST_GROUP(FIFO_Test);

static char *LOG;
static uint8_t LOG_len;

FIFO UUT;

enum {
    FIFO_SIZE = 64
};

TEST_SETUP(FIFO_Test)
{
    LOG = NULL;
    LOG_len = 0;

    UUT = FIFO_Create(FIFO_LOG_SIZE);
}

TEST_TEAR_DOWN(FIFO_Test)
{
    FIFO_Destroy(UUT);
}

static void FIFO_FILL(const char *text, uint8_t len)
{
    for (uint8_t entries = 0; entries < len; entries++)
        FIFO_Put(UUT, text);
}

static void FIFO_CLEAR(uint8_t len)
{
    for (uint8_t entries = 0; entries < len; entries++)
        FIFO_Get(UUT,&LOG, &LOG_len);

    LOG = NULL;
    LOG_len = 0;
}

IGNORE_TEST(FIFO_Test, puttingOrGettingFromNoInitThrowsError)
{
    FIFO_Destroy(UUT);

    TEST_ASSERT_EQUAL(FIFO_NO_INIT, FIFO_Put(UUT,"I like trains"));
    TEST_ASSERT_EQUAL(FIFO_NO_INIT, FIFO_Get(UUT,&LOG, &LOG_len));
}

TEST(FIFO_Test, gettingFromEmptyBufferThrowsError)
{
    TEST_ASSERT_EQUAL(FIFO_EMPTY, FIFO_Get(UUT,&LOG, &LOG_len));
}

TEST(FIFO_Test, NullLogThrowsError)
{
    TEST_ASSERT_EQUAL(FIFO_NULL_PTR, FIFO_Put(UUT,NULL));
}

TEST(FIFO_Test, FifoStoresAndReturnsOneLog)
{
    FIFO_Put(UUT,"Miau");
    FIFO_Get(UUT,&LOG, &LOG_len);

    TEST_ASSERT_EQUAL_CHAR_ARRAY("Miau", LOG, strlen("Miau"));
}

TEST(FIFO_Test, SuccessfullPutAndGetThrowsInfo)
{
    TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Put(UUT,"chika"));
    TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Get(UUT,&LOG, &LOG_len));
}

TEST(FIFO_Test, WontStoreMoreThanLogSize)
{
    char expected[FIFO_LOG_SIZE] = "Return where'd thou camst for thy is the place of belonging... \n";
    FIFO_Put(UUT,"Return where'd thou camst for thy is the place of belonging... YOU DIED");
    FIFO_Get(UUT,&LOG, &LOG_len);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, LOG, FIFO_LOG_SIZE);
}

TEST(FIFO_Test, FifoStoresMultipleLogsAndReturnsThemInCorrectOrder)
{
    FIFO_Put(UUT,"Dance");
    FIFO_Put(UUT,"car");
    FIFO_Put(UUT,"Assult");

    FIFO_Get(UUT,&LOG, &LOG_len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Dance", LOG, strlen("Dance"));

    FIFO_Get(UUT,&LOG, &LOG_len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("car", LOG, strlen("car"));

    FIFO_Get(UUT,&LOG, &LOG_len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Assult", LOG, strlen("Assult"));
}

TEST(FIFO_Test, PutAndGetEmptyThrow)
{
    FIFO_Put(UUT,"a");
    FIFO_Get(UUT,&LOG, &LOG_len);
    TEST_ASSERT_EQUAL(FIFO_EMPTY, FIFO_Get(UUT,&LOG, &LOG_len));
}

TEST(FIFO_Test, StoresDeclaredNumOfLogs)
{
    for (uint8_t entries = 0; entries < FIFO_SIZE; entries++)
        TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Put(UUT,"duck"));

    for (uint8_t entries = 0; entries < FIFO_SIZE; entries++)
        TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Get(UUT,&LOG, &LOG_len));
}

TEST(FIFO_Test, BufferOverFlowThrowsError)
{
    FIFO_FILL("b", FIFO_SIZE);

    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(UUT,"miau"));
}

TEST(FIFO_Test, AfterOverFlowSpecialLogIsQueued)
{
    FIFO_FILL("x", FIFO_SIZE);
    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(UUT,"chau"));
    FIFO_CLEAR(FIFO_SIZE);

    FIFO_Get(UUT,&LOG, &LOG_len);

    TEST_ASSERT_EQUAL_CHAR_ARRAY("***LOG OVERFLOW***\n", LOG, 19);
}

TEST(FIFO_Test, GetIndexOverFlow)
{
    FIFO_FILL("what does a fox say?", FIFO_SIZE);
    FIFO_CLEAR(FIFO_SIZE);
    FIFO_FILL("a", 2);
    FIFO_CLEAR(2);

    FIFO_Put(UUT,"I like big guns!");
    FIFO_Get(UUT,&LOG, &LOG_len);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("I like big guns!\n", LOG, 17);
}

TEST(FIFO_Test, MultiInstaceBehavior)
{
    FIFO secondQueue = FIFO_Create(FIFO_LOG_SIZE);
    FIFO_Put(secondQueue, "low key bad code");
    TEST_ASSERT_EQUAL(FIFO_EMPTY,FIFO_Get(UUT,&LOG,&LOG_len));
    FIFO_Destroy(secondQueue);
}

TEST(FIFO_Test, QueueSizeCanBeDetermined)
{
    FIFO secondQueue = FIFO_Create(1);
    FIFO_Put(secondQueue, "12345");
    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(secondQueue, "abcde"));

    FIFO_Destroy(secondQueue);
}

TEST(FIFO_Test, TextInSizeBoundsReturnsCorrectLen)
{
    FIFO_Put(UUT, "bam bam bam");
    FIFO_Get(UUT, &LOG, &LOG_len);

    TEST_ASSERT_EQUAL(12, LOG_len);
}

TEST(FIFO_Test, TextLenIsNeverGreaterThanMaxSize)
{
    FIFO_Put(UUT,"1234567876535678876543456789876543456887654345678987654345678744444"); //68
    FIFO_Get(UUT, &LOG, &LOG_len);
    TEST_ASSERT_EQUAL(FIFO_LOG_SIZE,LOG_len);
}

// TO DO:
TEST(FIFO_Test, MultipleOverFlowDontProduceMultipleSpecialLogs)
{
}
