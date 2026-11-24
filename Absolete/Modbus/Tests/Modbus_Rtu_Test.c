#include "unity_fixture.h"
#include "fff.h"
#include "Rtu.h"

TEST_GROUP(Modbus_Rtu);

static void sendInterface(uint8_t *data,uint8_t len);
DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(sendInterface,uint8_t*,uint8_t);

#define SLAVE_USED_ADDR (1u)

static uint16_t fakeValue = 0xAABB;

static uint8_t SlaveResponseBuffer[256] = {0};
static uint16_t SlaveResponseLen = 0;

void SlaveResponseSpy(uint8_t *data,uint8_t len)
{
    if(data == NULL || len == 0)
        return;

    memcpy(&SlaveResponseBuffer,data,len);
    SlaveResponseLen = len;
}

uint8_t setFakeValue(uint16_t val)
{
    fakeValue = val;
    return 1;
}

const Holding_Register_Called_Descriptor_t fakeValueDescriptor = {
    .ctx.offset     = 0,
    .ctx.valuePtr = &fakeValue,
    .setRegister = setFakeValue
};

const Holding_Register_Map_t FakeHoldingRegisterMap = {
    .Holding_Register_Called_Reference = &fakeValueDescriptor,
    .Holding_Register_Called_Count       = 1
};

const Modbus_Memory_Map_t fakeModbusMap = {
    .Holding_Registers = FakeHoldingRegisterMap
};

uint8_t read_first_holding_register_command[] = {SLAVE_USED_ADDR,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};

static void TEST_ASSERT_RESPONSE(uint8_t *expectedResponse,uint16_t expectedLen)
{
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);
    TEST_ASSERT_EQUAL(expectedLen,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedResponse,SlaveResponseBuffer,expectedLen);
}

TEST_SETUP(Modbus_Rtu)
{
    RESET_FAKE(sendInterface);
    FFF_RESET_HISTORY();
    Rtu_Slave_Init(SLAVE_USED_ADDR,sendInterface,&fakeModbusMap);
    memset(&SlaveResponseBuffer,0,256);
    SlaveResponseLen = 0;
    sendInterface_fake.custom_fake = SlaveResponseSpy;
}

TEST_TEAR_DOWN(Modbus_Rtu)
{
    Rtu_Slave_Destroy();
}

TEST(Modbus_Rtu,Init_SlaveAddrEqualZero_ReturnsError)
{
    Rtu_Slave_Destroy();
    TEST_ASSERT_EQUAL(RTU_INCORRECT_SLAVE_ADDR,Rtu_Slave_Init(0,sendInterface,&fakeModbusMap)); 
}

TEST(Modbus_Rtu,Init_InterfaceNULL_ReturnsError)
{
    Rtu_Slave_Destroy();
    TEST_ASSERT_EQUAL(RTU_SEND_INTERFACE_INVALID,Rtu_Slave_Init(SLAVE_USED_ADDR,NULL,&fakeModbusMap));
}

TEST(Modbus_Rtu,Init_MemoryMapIsNULL_ReturnsError)
{
    Rtu_Slave_Destroy();
    TEST_ASSERT_EQUAL(RTU_INIT_MEMORY_MAP_IS_NULL,Rtu_Slave_Init(SLAVE_USED_ADDR,sendInterface,NULL));
}

TEST(Modbus_Rtu,ProccessFrame_WithoutInit_ReturnsError)
{
    Rtu_Slave_Destroy();
    TEST_ASSERT_EQUAL(RTU_ERR_NO_INIT,Rtu_Interpret_Frame(NULL,11));
}

TEST(Modbus_Rtu,ProccessFrame_LenIsToShort_ReturnsError)
{
    TEST_ASSERT_EQUAL(RTU_ERR_FRAME_WRONG_LEN,Rtu_Interpret_Frame(read_first_holding_register_command,3));
}

TEST(Modbus_Rtu,ProccessFrame_LenIsToLong_ReturnsError)
{
    TEST_ASSERT_EQUAL(RTU_ERR_FRAME_WRONG_LEN,Rtu_Interpret_Frame(read_first_holding_register_command,257));
}

//data is null

TEST(Modbus_Rtu,ProccesFrame_WrongCRC_ReturnsError_DoesntRespond)
{
    uint8_t frameWithWrongCrc[] = {SLAVE_USED_ADDR,0x03,0x00,0x00,0x00,0x01,0x84,0x05};
    uint8_t frameLen = 8;

    TEST_ASSERT_EQUAL(RTU_ERR_WRONG_CRC,Rtu_Interpret_Frame(frameWithWrongCrc,frameLen));
    TEST_ASSERT_EQUAL(0,sendInterface_fake.call_count);
}

TEST(Modbus_Rtu,ProccessFrame_WrongId_IgnoresMessage)
{
    uint8_t frameWithWrongSlaveId[] = {SLAVE_USED_ADDR+ 1u,0x03,0x00,0x00,0x00,0x01,0x84,0x39};
    uint8_t frameLen = sizeof(frameWithWrongSlaveId);

    TEST_ASSERT_EQUAL(RTU_IGNORE, Rtu_Interpret_Frame(frameWithWrongSlaveId,frameLen));
    TEST_ASSERT_EQUAL(0,sendInterface_fake.call_count);
}

TEST(Modbus_Rtu,ProccessFrame_IllegalFunctionCode_SendsExcepetion)
{
    uint8_t frameWithIllegalFunction[] = {SLAVE_USED_ADDR,0x00,0x00,0x00,0x00,0x01,0xC0,0x0A};
    uint8_t frameLen = sizeof(frameWithIllegalFunction);

    TEST_ASSERT_EQUAL(RTU_EXCEPTION_ILLEGAL_FUNCTION,Rtu_Interpret_Frame(frameWithIllegalFunction,frameLen));
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);
    uint8_t expectedFrame[] = {SLAVE_USED_ADDR,0x80,0x01,0x80,0x00};
    uint8_t expectedLen = 5;
    TEST_ASSERT_EQUAL(5,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedFrame,SlaveResponseBuffer,expectedLen);
}

TEST(Modbus_Rtu,ProccessFrame_GetZeroValues_SendsException)
{
    uint8_t frameGetZeroValues[] = {SLAVE_USED_ADDR,0x03,0x00,0x00,0x00,0x00,0x45,0xCA};
    uint8_t frameLen = sizeof(frameGetZeroValues);

    TEST_ASSERT_EQUAL(RTU_EXCEPTION_ILLEGAL_DATA_VALUE,Rtu_Interpret_Frame(frameGetZeroValues,frameLen));
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);
    uint8_t expectedFrame[] = {SLAVE_USED_ADDR,0x83,0x03,0x01,0x31};
    uint8_t expectedLen = 5;
    TEST_ASSERT_EQUAL(5,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedFrame,SlaveResponseBuffer,expectedLen);
    //check if frame is parsed correctly
}

TEST(Modbus_Rtu,ProccessFrame_GetMultipleValues_IfNotDefinedSendException)
{
    uint8_t frameGetMultipleValues[] = {SLAVE_USED_ADDR,0x03,0x00,0x00,0x00,0x05,0x85,0xC9};
    uint8_t frameLen = sizeof(frameGetMultipleValues);

    TEST_ASSERT_EQUAL(RTU_EXCEPTION_SERVER_DEVICE_FAILURE,Rtu_Interpret_Frame(frameGetMultipleValues,frameLen));
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);

    uint8_t expectedResponse[] = {SLAVE_USED_ADDR, 0x83,0x04,0x40,0xF3};
    uint8_t expectedResponseLen = 5;

    TEST_ASSERT_EQUAL(expectedResponseLen,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedResponse,SlaveResponseBuffer,expectedResponseLen);
}

TEST(Modbus_Rtu,ProccessFrame_GetNotDefinedOffset_SendsException)
{
    uint8_t frameWithWrongOffset[] = {SLAVE_USED_ADDR, 0x03, 0xAA,0x00,0x00,0x01,0xA5,0xD2};
    uint8_t frameLen = sizeof(frameWithWrongOffset);

    TEST_ASSERT_EQUAL(RTU_EXCEPTION_ILLEGAL_DATA_ADDRESS,Rtu_Interpret_Frame(frameWithWrongOffset,frameLen));
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);

    uint8_t expectedFrame[] = {SLAVE_USED_ADDR,0x83,0x02,0xC0,0xF1};
    uint8_t expectedLen = sizeof(expectedFrame);

    TEST_ASSERT_EQUAL(expectedLen,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedFrame,SlaveResponseBuffer,expectedLen);
}

TEST(Modbus_Rtu,ProccessFrame_HappyPath_ReturnsValue)
{
    uint8_t readOneRegister[] = {SLAVE_USED_ADDR,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
    uint8_t frameLen = sizeof(readOneRegister);

    TEST_ASSERT_EQUAL(RTU_OK,Rtu_Interpret_Frame(readOneRegister,frameLen));
    TEST_ASSERT_EQUAL(1,sendInterface_fake.call_count);

    uint8_t expectedFrame[] = {SLAVE_USED_ADDR,0x03,0x02,0xAA,0xBB,0x86,0x97};
    uint8_t expectedLen = sizeof(expectedFrame);

    TEST_ASSERT_EQUAL(expectedLen,SlaveResponseLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedFrame,SlaveResponseBuffer,expectedLen);
}