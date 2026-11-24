#ifndef RTU_H
#define RTU_H

#include "stdint.h"

//zalozenia
//aplikacja zewnetrzna / BSP zajmuje sie oczekiwaniem na 3.5 znaku
//aplikacja zewnetrzna / BSP przekazuje interfejs do wsylania odpowiedzi
//Modul zajmuje sie wysylaniem ramek odpowiedzi poprzez interfejs
//Modul nie przechowuje wartosci pamieci tylko deskryptor
//jezeli callback istnieje uzywamy go
//jezeli callback ustawiajacy wartosc nie istnieje korzystamy z memcpy i memset

#define ALLOW_READ_MULTIPLE_REGISTERS (0u)


typedef enum {
    RTU_OK,
    RTU_IGNORE,
    RTU_INCORRECT_SLAVE_ADDR,
    RTU_SEND_INTERFACE_INVALID,
    RTU_INIT_MEMORY_MAP_IS_NULL,
    RTU_ERR_NO_INIT,
    RTU_ERR_FRAME_WRONG_LEN,
    RTU_ERR_WRONG_CRC,
    RTU_EXCEPTION_ILLEGAL_FUNCTION,
    RTU_EXCEPTION_ILLEGAL_DATA_VALUE,
    RTU_EXCEPTION_ILLEGAL_DATA_ADDRESS,
    RTU_EXCEPTION_SERVER_DEVICE_FAILURE
} Rtu_Status_t;

typedef struct {
    uint16_t * const valuePtr;
    const uint16_t offset;
} Holding_Register_Ctx_t;

typedef struct {
    const Holding_Register_Ctx_t ctx;
    uint8_t   (*setRegister)(uint16_t value);
} Holding_Register_Called_Descriptor_t; //nazwa do zmiany

// typedef struct {
//     uint16_t * const valuePtr;
//     const uint16_t offset;
//     uint16_t * const acceptableValuesList;
//     const uint16_t acceptableValuesList_Len;
// } Holding_Register_Enumerated_Descriptor_t;

// typedef struct {
//     uint16_t * const  valuePtr;
//     const uint16_t  offset;
//     const uint16_t  upperBound;
//     const uint16_t  lowerBound;
// } Holding_Register_Bounded_Descriptor_t;

typedef struct {
    const Holding_Register_Called_Descriptor_t * const Holding_Register_Called_Reference;
    const uint16_t Holding_Register_Called_Count;
    // Holding_Register_Enumerated_Descriptor_t * const Holding_Register_Enumerated_Reference;
    // const uint16_t Holding_Register_Enumerated_Count;
    // Holding_Register_Bounded_Descriptor_t * const Holding_Register_Bounded_Reference;
    // const uint16_t Holding_register_Bounded_Count;
} Holding_Register_Map_t;

typedef struct {
    const Holding_Register_Map_t Holding_Registers;
} Modbus_Memory_Map_t;

Rtu_Status_t Rtu_Slave_Init(uint8_t setSlaveAddress, void(*sendFrameInterface)(uint8_t *data,uint8_t len),const Modbus_Memory_Map_t * setMemoryMap);
Rtu_Status_t Rtu_Interpret_Frame(uint8_t * data, uint16_t len);
Rtu_Status_t Rtu_Slave_Destroy();

#endif