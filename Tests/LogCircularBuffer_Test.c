#include "unity_fixture.h"
#include "unity.h"

#include "LogCircularBuffer.h"

#include <string.h>

#define BUFFER_SIZE 40

TEST_GROUP(LogCircularBuffer);

static char buffer[BUFFER_SIZE];
static const int UUT_BufferOffset = 10;
static const int UUT_BufferSize = 20;

static char * receivedPtr;
static uint32_t receivedLength;

TEST_SETUP(LogCircularBuffer)
{
    memset(buffer,0xAA,sizeof(buffer));

    receivedPtr = NULL;
    receivedLength = 9999;

    LogCircularBuffer_Init(buffer+UUT_BufferOffset,UUT_BufferSize);
}

TEST_TEAR_DOWN(LogCircularBuffer)
{
}

TEST(LogCircularBuffer, BufferAfterInitContainsNothing)
{
    for(int index = 0; index < UUT_BufferSize; index++)
        TEST_ASSERT_EQUAL_CHAR(' ',buffer[UUT_BufferOffset + index]);
}

TEST(LogCircularBuffer, GettingFromNoInitBufferThrowsError)
{
    LogCircularBuffer_Destroy();
    TEST_ASSERT_EQUAL(BUFFER_NO_INIT,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
}

TEST(LogCircularBuffer, PuttingToNoInitBufferThrowError)
{
    LogCircularBuffer_Destroy();
    char text[] = "test\0";

    TEST_ASSERT_EQUAL(BUFFER_NO_INIT,LogCircularBuffer_Put(text));
}

TEST(LogCircularBuffer, BufferStoresOneItem)
{

    LogCircularBuffer_Put("text");

    TEST_ASSERT_EQUAL_STRING("text\0",buffer + UUT_BufferOffset);
}

TEST(LogCircularBuffer, BufferStoresMultipleItems)
{
    LogCircularBuffer_Put("data1");
    LogCircularBuffer_Put("data2");

    TEST_ASSERT_EQUAL_CHAR_ARRAY("data1\0data2\0", buffer + UUT_BufferOffset,12);
}

TEST(LogCircularBuffer, BufferDoesntCurruptMemory)
{
    LogCircularBuffer_Put("123456789012345678901234567890");

    for(int index = 0; index < 10;index ++)
    {
        TEST_ASSERT_EQUAL_CHAR(0xAA,buffer[index]);
        TEST_ASSERT_EQUAL_CHAR(0xaa,buffer[UUT_BufferOffset+UUT_BufferSize+index]);
    }
}

TEST(LogCircularBuffer, BufferTruncatesMessages)
{
    LogCircularBuffer_Put("123456789012345678901234567890");

    TEST_ASSERT_EQUAL_STRING("1234567890123456789",buffer+UUT_BufferOffset);
}

TEST(LogCircularBuffer, BufferThrowsErrorWhenGettingFromEmpty)
{
    TEST_ASSERT_EQUAL(BUFFER_EMPTY,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
}

TEST(LogCircularBuffer, BufferReturnsGivenItemAndItsLength)
{
    LogCircularBuffer_Put("Miau");

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);

    TEST_ASSERT_EQUAL_CHAR_ARRAY("Miau\0",receivedPtr,5);
    TEST_ASSERT_EQUAL(5,receivedLength);
}

TEST(LogCircularBuffer, BufferReturnsSameDataIfNotEndOfProccesed)
{
    LogCircularBuffer_Put("Eryk");
    LogCircularBuffer_Put("desk");

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Eryk\0",receivedPtr,5);
    TEST_ASSERT_EQUAL(5,receivedLength);

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Eryk\0",receivedPtr,5);
    TEST_ASSERT_EQUAL(5,receivedLength);
}

TEST(LogCircularBuffer, AdvancingClearsUnusedIndexes)
{
    LogCircularBuffer_Put("melatonin");
    LogCircularbuffer_Get(&receivedPtr,&receivedLength);

    LogCircularBuffer_AdvanceGetIndex();

    for(int index = 0; index < UUT_BufferSize; index++)
        TEST_ASSERT_EQUAL_CHAR(' ',buffer[UUT_BufferOffset+index]);
}

TEST(LogCircularBuffer, BufferReturnsItemsInCorrectOrder)
{
    LogCircularBuffer_Put("Eryk");
    LogCircularBuffer_Put("rockie");

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Eryk\0",receivedPtr,5);
    TEST_ASSERT_EQUAL(5,receivedLength);

    LogCircularBuffer_AdvanceGetIndex();

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("rockie\0",receivedPtr,7);
    TEST_ASSERT_EQUAL_INT(7,receivedLength);
}



TEST(LogCircularBuffer, BufferDoesntReturnFullLogOnEdge)
{
    LogCircularBuffer_Put("test");
    LogCircularBuffer_Put("testing is very benificient");

    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    LogCircularBuffer_AdvanceGetIndex();

    TEST_ASSERT_EQUAL(LOG_FULL,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("testing is ver\0",receivedPtr,15);
    TEST_ASSERT_EQUAL(15,receivedLength);
}

TEST(LogCircularBuffer, BufferReturnsWrappedLogInTwoTries)
{
    LogCircularBuffer_Put("test");
    LogCircularbuffer_Get(&receivedPtr,&receivedLength);
    LogCircularBuffer_AdvanceGetIndex();

    LogCircularBuffer_Put("I very like apples");


    TEST_ASSERT_EQUAL(LOG_SPLITTED,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("I very like app",receivedPtr,15);
    TEST_ASSERT_EQUAL(15,receivedLength);

    LogCircularBuffer_AdvanceGetIndex();
    TEST_ASSERT_EQUAL(LOG_FULL,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("les\0",receivedPtr,4);
    TEST_ASSERT_EQUAL(4,receivedLength);
}

TEST(LogCircularBuffer, PuttingIntoFullBufferReturnsError)
{
    // 19 chars + terminator == UUT_BufferSize, i.e. zero sentinel gap left
    LogCircularBuffer_Put("1234567890123456789");

    TEST_ASSERT_EQUAL(BUFFER_FULL,LogCircularBuffer_Put("x"));
}

TEST(LogCircularBuffer, TerminatorLandingOnLastByteReturnsFull)
{
    LogCircularBuffer_Put("1234567890123456789");
    TEST_ASSERT_EQUAL(LOG_FULL,LogCircularbuffer_Get(&receivedPtr,&receivedLength));
}

TEST(LogCircularBuffer, PutReturnsSuccessOnNormalWrite)
{
    TEST_ASSERT_EQUAL(BUFFER_SUCCES, LogCircularBuffer_Put("ok"));
}

TEST(LogCircularBuffer, AdvanceGetIndexWithNothingPendingIsSafe)
{
    LogCircularBuffer_AdvanceGetIndex();   // no prior successful Get()
    TEST_ASSERT_EQUAL(BUFFER_EMPTY, LogCircularbuffer_Get(&receivedPtr,&receivedLength));
}

