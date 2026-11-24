#include "Rtu.h" 
#include "CRC16.h"
#include "string.h"

#define BROADCAST_ADDR      (0u)
#define RTU_FRAME_MAX_LEN (256u)
#define RTU_FRAME_MIN_LEN  (4u)
#define CRC_LEN                       (2u)
#define FUNCTIONS_IMPLEMENTED (1u)
#define READ_HOLDING_REGISTERS (3u)
#define MAX_GET_HOLDING_REGISTERS (125u)
#define RTU_EXCEPTION_FRAME_LEN (5u)

static uint8_t slaveAddress;
static void     (*sendFrameFun)(uint8_t*data,uint8_t len);
static uint8_t isInitialised;
static Modbus_Memory_Map_t* memoryMap;

typedef struct {
    uint8_t functionId;
    Rtu_Status_t (*functionCallback)(uint8_t *data,uint8_t len); //whole frame is parsed here
} Function_t;

typedef enum {
    ILLEGAL_FUNCTION         = 0x01,
    ILLEGAL_DATA_ADDRESS = 0x02,
    ILLEGAL_DATA_VALUE      = 0x03,
    SERVER_DEVICE_FAILURE = 0x04
} Rtu_Exception_t;

static Rtu_Status_t ReadHoldingRegistersStrategy(uint8_t *data,uint8_t len);
static void AddCRC(uint8_t *frameWithoutCrc,uint16_t lenWithoutCrc);
void SendException(Rtu_Exception_t exception, uint8_t functionCode);
static Rtu_Status_t ValidateFrame(uint8_t* data,uint16_t len);

static const Function_t functionList[FUNCTIONS_IMPLEMENTED] = {
    {
        .functionCallback = ReadHoldingRegistersStrategy,
        .functionId = 0x03
    }
}; //Basicowe funkcje moga byc we flash wiec static const

Rtu_Status_t Rtu_Slave_Init(uint8_t setSlaveAddress, void(*sendFrameInterface)(uint8_t *data,uint8_t len),const Modbus_Memory_Map_t * setMemoryMap)
{
    if(setSlaveAddress == BROADCAST_ADDR)  return RTU_INCORRECT_SLAVE_ADDR;
    if(sendFrameInterface == NULL)                   return RTU_SEND_INTERFACE_INVALID;
    if(setMemoryMap == NULL)                         return RTU_INIT_MEMORY_MAP_IS_NULL;
        
    slaveAddress   = setSlaveAddress;
    sendFrameFun = sendFrameInterface;
    memoryMap   = setMemoryMap;
    isInitialised       = 1;

    return RTU_OK;
}

Rtu_Status_t Rtu_Interpret_Frame(uint8_t * data, uint16_t len)
{
    if(!isInitialised)  return RTU_ERR_NO_INIT;
    
    Rtu_Status_t validationStatus = ValidateFrame(data,len);
    if(validationStatus != RTU_OK) return validationStatus;

    for(uint8_t index = 0; index < FUNCTIONS_IMPLEMENTED; index++)
    {
        if(data[1] == functionList[index].functionId)
            return functionList[index].functionCallback(data,len);
    }

    SendException(ILLEGAL_FUNCTION,data[1]);
    return RTU_EXCEPTION_ILLEGAL_FUNCTION;
}

void SendException(Rtu_Exception_t exception,uint8_t functionCode)
{
    uint8_t exceptionFrame[RTU_EXCEPTION_FRAME_LEN];
    exceptionFrame[0] = slaveAddress;
    exceptionFrame[1] = functionCode + 0x80;
    exceptionFrame[2] = exception;
    AddCRC(exceptionFrame,3);

    sendFrameFun(exceptionFrame,RTU_EXCEPTION_FRAME_LEN);
}

Rtu_Status_t ReadHoldingRegistersStrategy(uint8_t *data,uint8_t len)
{
    uint16_t numRequestedRegisters = data[4] << 8 | data[5];

    if(numRequestedRegisters == 0 || numRequestedRegisters > MAX_GET_HOLDING_REGISTERS)
    {
        SendException(ILLEGAL_DATA_VALUE,READ_HOLDING_REGISTERS);
        return RTU_EXCEPTION_ILLEGAL_DATA_VALUE;
    }
    #if ALLOW_READ_MULTIPLE_REGISTERS == 0
    if(numRequestedRegisters > 1)
    {
        SendException(SERVER_DEVICE_FAILURE,READ_HOLDING_REGISTERS);
        return RTU_EXCEPTION_SERVER_DEVICE_FAILURE;
    }
    #endif
    uint16_t offset = (uint16_t)data[2] << 8 | data[3];

    uint8_t response[RTU_FRAME_MAX_LEN];
    uint8_t responseLen = 3+numRequestedRegisters*2 + CRC_LEN;
    response[0] = slaveAddress;
    response[1] = data[1];
    response[2] = (uint8_t)numRequestedRegisters*2;
    for(uint16_t index = 0; index < memoryMap-> Holding_Registers.Holding_Register_Called_Count;index++ )
    {
        if(memoryMap->Holding_Registers.Holding_Register_Called_Reference->ctx.offset == offset)
        {
            uint16_t val;
            memcpy(&val,memoryMap->Holding_Registers.Holding_Register_Called_Reference->ctx.valuePtr,2);
            response[3] = (uint8_t)(val >> 8);
            response[4] = (uint8_t)(val & 0x00FFu);
            AddCRC(response,responseLen-CRC_LEN);
            sendFrameFun(response,responseLen);
            return RTU_OK;
        }
    }

    SendException(ILLEGAL_DATA_ADDRESS,READ_HOLDING_REGISTERS);
    return RTU_EXCEPTION_ILLEGAL_DATA_ADDRESS;
}

static Rtu_Status_t ValidateFrame(uint8_t* data,uint16_t len)
{
    uint16_t frameCRC= data[len-1] << 8 | data[len-CRC_LEN]; //Little Endian
    
    if(len < RTU_FRAME_MIN_LEN || len > RTU_FRAME_MAX_LEN) return RTU_ERR_FRAME_WRONG_LEN;
    if(frameCRC != CRC16(data,len - CRC_LEN))                             return RTU_ERR_WRONG_CRC;
    if(data[0] != slaveAddress && data[0] != BROADCAST_ADDR) return RTU_IGNORE;
        
    return RTU_OK;
}

static void AddCRC(uint8_t *frameWithoutCrc,uint16_t lenWithoutCrc)
{
    uint16_t CRC    = CRC16(frameWithoutCrc,lenWithoutCrc);
    uint8_t lowCRC  = (uint8_t)(CRC & 0x00FFu); // młodszy bajt (bity 0..7)
    uint8_t hiCRC   = (uint8_t)(CRC >> 8);           // starszy bajt (bity 8..15)
    frameWithoutCrc[lenWithoutCrc++] = lowCRC;
    frameWithoutCrc[lenWithoutCrc] = hiCRC;
}

Rtu_Status_t Rtu_Slave_Destroy()
{
    isInitialised = 0;
    sendFrameFun = NULL;
    return RTU_OK;
}