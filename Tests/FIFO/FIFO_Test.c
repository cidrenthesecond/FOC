#include "unity_fixture.h"
#include "unity.h"
#include "string.h"

#include "FIFO.h"

TEST_GROUP(FIFO_Test);

FIFO UUT;

static char *LOG;
static uint8_t LOG_len_received;

static void SPY_INIT()
{
    LOG = NULL;
    LOG_len_received = 0;
}
static int SPY_ACQUIRE_LOG()
{
    LOG = NULL;
    LOG_len_received = 0;
    return FIFO_Get(UUT,&LOG,&LOG_len_received);
}
static char* SPY_GET_LOG()
{
    char * result = LOG;
    LOG = NULL;
    return result;
}
static uint8_t SPY_GET_LEN()
{
    uint8_t result = LOG_len_received;
    LOG_len_received = 0;
    return result;
}

#define FIFO_SIZE 64

TEST_SETUP(FIFO_Test)
{
    LOG = NULL;
    LOG_len_received = 0;

    SPY_INIT();
    UUT = FIFO_Create(FIFO_MAX_LOG_LENGTH);
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
        FIFO_Get(UUT,&LOG, &LOG_len_received);

    LOG = NULL;
    LOG_len_received = 0;
}

IGNORE_TEST(FIFO_Test, puttingOrGettingFromNoInitThrowsError)
{
    FIFO_Destroy(UUT);

    TEST_ASSERT_EQUAL(FIFO_NO_INIT, FIFO_Put(UUT,"I like trains"));
    TEST_ASSERT_EQUAL(FIFO_NO_INIT, SPY_ACQUIRE_LOG());
}

TEST(FIFO_Test, InitWithZeroLengthReturnsNULL)
{
    FIFO_Destroy(UUT);

    UUT = FIFO_Create(0);
    TEST_ASSERT_EQUAL(NULL,UUT);
}

TEST(FIFO_Test, CallingDestroyMoreThanOnceIsSafe)
{
    FIFO_Destroy(UUT);
    FIFO_Destroy(UUT);
    FIFO_Destroy(UUT);
}

TEST(FIFO_Test, gettingFromEmptyBufferThrowsError)
{
    TEST_ASSERT_EQUAL(FIFO_EMPTY, SPY_ACQUIRE_LOG());
}

TEST(FIFO_Test, NullLogThrowsError)
{
    TEST_ASSERT_EQUAL(FIFO_NULL_PTR, FIFO_Put(UUT,NULL));
}

TEST(FIFO_Test, FifoStoresAndReturnsOneLog)
{
    FIFO_Put(UUT,"Miau");
    SPY_ACQUIRE_LOG();

    TEST_ASSERT_EQUAL_CHAR_ARRAY("Miau", SPY_GET_LOG(), strlen("Miau"));
}

TEST(FIFO_Test, FIFO_Get_ReturnsPushedLogLength)
{
    FIFO_Put(UUT, "bam bam bam");
    SPY_ACQUIRE_LOG();

    TEST_ASSERT_EQUAL(12, SPY_GET_LEN());
}

TEST(FIFO_Test, SuccessfullPutAndGetThrowsInfo)
{
    TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Put(UUT,"chika"));
    TEST_ASSERT_EQUAL(FIFO_SUCCESS, SPY_ACQUIRE_LOG());
}

TEST(FIFO_Test, Put_LogExceedingMaxLength_TruncatesContent)
{
    char expected[FIFO_MAX_LOG_LENGTH] = "Return where'd thou camst for thy is the place of belonging... \n";
    FIFO_Put(UUT,"Return where'd thou camst for thy is the place of belonging... YOU DIED");
    FIFO_Get(UUT,&LOG, &LOG_len_received);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, LOG, FIFO_MAX_LOG_LENGTH);
}

TEST(FIFO_Test, Get_WhenInputExceedsMaxCapacity_ReceivedLengthEqualsMaxLength)
{
    FIFO_Put(UUT,"1234567876535678876543456789876543456887654345678987654345678744444"); //68
    FIFO_Get(UUT, &LOG, &LOG_len_received);
    TEST_ASSERT_EQUAL(FIFO_MAX_LOG_LENGTH,LOG_len_received);
}

TEST(FIFO_Test, FifoStoresMultipleLogsAndReturnsThemInCorrectOrder)
{
    FIFO_Put(UUT,"Dance");
    FIFO_Put(UUT,"car");
    FIFO_Put(UUT,"Assult");

    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Dance", SPY_GET_LOG(), strlen("Dance"));
    TEST_ASSERT_EQUAL(6,SPY_GET_LEN());

    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL_CHAR_ARRAY("car", SPY_GET_LOG(), strlen("car"));
    TEST_ASSERT_EQUAL(4,SPY_GET_LEN());

    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Assult", SPY_GET_LOG(), strlen("Assult"));
    TEST_ASSERT_EQUAL(7,SPY_GET_LEN());
}

TEST(FIFO_Test, Get_FromEmptyBuffer_ReturnsFifoEmpty)
{
    FIFO_Put(UUT,"a");
    FIFO_Get(UUT,&LOG, &LOG_len_received);
    TEST_ASSERT_EQUAL(FIFO_EMPTY, SPY_ACQUIRE_LOG());
}

TEST(FIFO_Test, StoresDeclaredNumOfLogs)
{
    for (uint8_t entries = 0; entries < FIFO_SIZE; entries++)
        TEST_ASSERT_EQUAL(FIFO_SUCCESS, FIFO_Put(UUT,"duck"));

    for (uint8_t entries = 0; entries < FIFO_SIZE; entries++)
        TEST_ASSERT_EQUAL(FIFO_SUCCESS, SPY_ACQUIRE_LOG());
}

TEST(FIFO_Test, BufferOverFlowThrowsError)
{
    FIFO_FILL("b", FIFO_SIZE);

    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(UUT,"miau"));
}

TEST(FIFO_Test, Put_WhenBufferOverflows_QueuesOverflowWarningMessage)
{
    FIFO_FILL("x", FIFO_SIZE);
    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(UUT,"chau"));
    FIFO_CLEAR(FIFO_SIZE);

    SPY_ACQUIRE_LOG();

    TEST_ASSERT_EQUAL_CHAR_ARRAY("***LOG OVERFLOW***\n", SPY_GET_LOG(), 19);
}

TEST(FIFO_Test, OverFlowLogCorrectLengthIsReturned)
{
    FIFO_FILL("FILL",FIFO_SIZE);
    TEST_ASSERT_EQUAL(FIFO_FULL,FIFO_Put(UUT,"FILL"));
    FIFO_CLEAR(FIFO_SIZE);

    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL(19,SPY_GET_LEN());
}

TEST(FIFO_Test, Put_WhenBufferIsFull_ReturnsFifoFullAndRejectsData)
{
    FIFO_FILL("FILL",FIFO_SIZE);

    TEST_ASSERT_EQUAL(FIFO_FULL,FIFO_Put(UUT,"That One Overflows"));
    TEST_ASSERT_EQUAL(FIFO_FULL,FIFO_Put(UUT,"That One tries to get to Sentinel"));
    TEST_ASSERT_EQUAL(FIFO_FULL,FIFO_Put(UUT,"That One tries to save on index: 1"));

    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL_CHAR_ARRAY("FILL\n",SPY_GET_LOG(),5);
}

TEST(FIFO_Test, Get_AfterRepeatedOverflow_QueuesOverflowWarningMessageAgain)
{
    FIFO_FILL("FILL",FIFO_SIZE);
    FIFO_Put(UUT,"First OverFlow");
    SPY_ACQUIRE_LOG();

    TEST_ASSERT_EQUAL(FIFO_FULL,FIFO_Put(UUT,"Second OverFlow"));
    FIFO_CLEAR(FIFO_SIZE - 1);
    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL_CHAR_ARRAY("***LOG OVERFLOW***\n",LOG,19);
    TEST_ASSERT_EQUAL(FIFO_SUCCESS,SPY_ACQUIRE_LOG());
    TEST_ASSERT_EQUAL_CHAR_ARRAY("***LOG OVERFLOW***\n",LOG,19);
}

TEST(FIFO_Test, GetIndexOverFlow)
{
    FIFO_FILL("what does a fox say?", FIFO_SIZE);
    FIFO_CLEAR(FIFO_SIZE);
    FIFO_FILL("a", 2);
    FIFO_CLEAR(2);

    FIFO_Put(UUT,"I like big guns!");
    FIFO_Get(UUT,&LOG, &LOG_len_received);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("I like big guns!\n", LOG, 17);
}

TEST(FIFO_Test, MultipleInstances_MaintainSeparateBuffers)
{
    FIFO secondQueue = FIFO_Create(FIFO_MAX_LOG_LENGTH);

    FIFO_Put(secondQueue, "low key bad code");

    TEST_ASSERT_EQUAL(FIFO_EMPTY,FIFO_Get(UUT,&LOG,&LOG_len_received));

    FIFO_Destroy(secondQueue);
}

TEST(FIFO_Test, QueueSizeCanBeDetermined)
{
    FIFO secondQueue = FIFO_Create(1);
    FIFO_Put(secondQueue, "12345");
    TEST_ASSERT_EQUAL(FIFO_FULL, FIFO_Put(secondQueue, "abcde"));

    FIFO_Destroy(secondQueue);
}

TEST(FIFO_Test, PutGetPutLength)
{
    FIFO_Put(UUT,"dummy1");
    SPY_ACQUIRE_LOG();
    FIFO_Put(UUT, "nya");
    SPY_ACQUIRE_LOG();
    TEST_ASSERT_EQUAL(4,SPY_GET_LEN());
}


