#include "unity_fixture.h"

TEST_GROUP_RUNNER(Modbus_Rtu)
{
    RUN_TEST_CASE(Modbus_Rtu,Init_SlaveAddrEqualZero_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,Init_InterfaceNULL_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,Init_MemoryMapIsNULL_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_WithoutInit_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_LenIsToShort_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_LenIsToLong_ReturnsError);
    RUN_TEST_CASE(Modbus_Rtu,ProccesFrame_WrongCRC_ReturnsError_DoesntRespond);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_WrongId_IgnoresMessage);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_IllegalFunctionCode_SendsException);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_GetZeroValues_SendsException);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_GetMultipleValues_IfNotDefinedSendException);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_GetNotDefinedOffset_SendsException);
    RUN_TEST_CASE(Modbus_Rtu,ProccessFrame_HappyPath_ReturnsValue);
}