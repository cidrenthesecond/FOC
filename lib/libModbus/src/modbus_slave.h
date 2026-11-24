#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include "modbus_bytes.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MODBUS_MIN_FRAME_LENGTH 4
#define MODBUS_MAX_FRAME_LENGTH 256
#define MODBUS_MAX_PDU_LENGTH   253
#define MODBUS_FC_EXCEPTION_MASK 0x80

#define MODBUS_MEI_TYPE_READ_DEVICE_ID 0x0E

#define MODBUS_READ_DEVICE_ID_BASIC      0x01
#define MODBUS_READ_DEVICE_ID_REGULAR    0x02
#define MODBUS_READ_DEVICE_ID_EXTENDED   0x03
#define MODBUS_READ_DEVICE_ID_INDIVIDUAL 0x04

#define MODBUS_DEVICE_ID_VENDOR_NAME          0x00
#define MODBUS_DEVICE_ID_PRODUCT_CODE         0x01
#define MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION 0x02
#define MODBUS_DEVICE_ID_VENDOR_URL           0x03
#define MODBUS_DEVICE_ID_PRODUCT_NAME         0x04
#define MODBUS_DEVICE_ID_MODEL_NAME           0x05
#define MODBUS_DEVICE_ID_USER_APPLICATION     0x06

#define MODBUS_DEVICE_ID_CONFORMITY_BASIC_INDIVIDUAL    0x81
#define MODBUS_DEVICE_ID_CONFORMITY_REGULAR_INDIVIDUAL  0x82
#define MODBUS_DEVICE_ID_CONFORMITY_EXTENDED_INDIVIDUAL 0x83

/*==============================
    Modbus state machine
==============================*/
typedef enum {
    UNINITIALIZED = 0,
    IDLE,
    RECEPTION,
    CONTROL_AND_WAITING,
} ModbusState;

/*==============================
    Function codes
==============================*/
typedef enum {
    MODBUS_FC_READ_COILS                = 0x01,
    MODBUS_FC_READ_DISCRETE_INPUTS      = 0x02,
    MODBUS_FC_READ_HOLDING_REGISTERS    = 0x03,
    MODBUS_FC_READ_INPUT_REGISTERS      = 0x04,

    MODBUS_FC_WRITE_SINGLE_COIL         = 0x05,
    MODBUS_FC_WRITE_SINGLE_REGISTER     = 0x06,

    MODBUS_FC_WRITE_MULTIPLE_COILS      = 0x0F,
    MODBUS_FC_WRITE_MULTIPLE_REGISTERS  = 0x10,

    MODBUS_FC_MASK_WRITE_REGISTER       = 0x16,
    MODBUS_FC_READ_WRITE_MULTIPLE_REGS  = 0x17,

    MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT    = 0x2B,
} ModbusFunctionCode;

/*==============================
    Exception codes
==============================*/
typedef enum {
    MODBUS_EX_NONE                 = 0x00, /* No exception */
    MODBUS_EX_ILLEGAL_FUNCTION     = 0x01, /* Function not supported */
    MODBUS_EX_ILLEGAL_DATA_ADDRESS = 0x02, /* Invalid register/coil address */
    MODBUS_EX_ILLEGAL_DATA_VALUE   = 0x03, /* Invalid value */
    MODBUS_EX_SLAVE_DEVICE_FAILURE = 0x04, /* Device failure */
} ModbusExceptionCode;

/*==============================
    Callback typedefs
==============================*/
typedef ModbusExceptionCode (*ModbusReadCoilsCb)(uint16_t addr, uint16_t count, uint8_t *dest);
typedef ModbusExceptionCode (*ModbusReadDiscreteInputsCb)(uint16_t addr, uint16_t count, uint8_t *dest);
typedef ModbusExceptionCode (*ModbusReadHoldingRegistersCb)(uint16_t addr, uint16_t count, uint8_t *dest);
typedef ModbusExceptionCode (*ModbusReadInputRegistersCb)(uint16_t addr, uint16_t count, uint8_t *dest);

typedef ModbusExceptionCode (*ModbusWriteSingleCoilCb)(uint16_t addr, uint16_t value);
typedef ModbusExceptionCode (*ModbusWriteSingleRegisterCb)(uint16_t addr, uint16_t value);
typedef ModbusExceptionCode (*ModbusWriteMultipleCoilsCb)(uint16_t addr, uint16_t count, const uint8_t *src);
typedef ModbusExceptionCode (*ModbusWriteMultipleRegistersCb)(uint16_t addr, uint16_t count, const uint8_t *src);

typedef ModbusExceptionCode (*ModbusMaskWriteRegisterCb)(uint16_t addr, uint16_t and_mask, uint16_t or_mask);
typedef ModbusExceptionCode (*ModbusReadWriteMultipleRegistersCb)(
    uint16_t read_addr, uint16_t read_count,
    uint16_t write_addr, uint16_t write_count,
    const uint8_t *write_data, uint8_t *read_data
);

typedef ModbusExceptionCode (*ModbusUserFunctionCb)(
    uint8_t function_code,
    const uint8_t *request,
    uint16_t request_len,
    uint8_t *response,
    uint16_t response_max,
    uint16_t *response_len,
    bool *send_response,
    void *user_ctx
);

typedef struct {
    uint8_t function_code;
    ModbusUserFunctionCb handler;
} ModbusUserFunctionHandler;

typedef struct {
    uint8_t object_id;
    const char *value;
} ModbusDeviceIdObject;

/*==============================
    Configuration
==============================*/
typedef struct {
    uint8_t address;

    void (*write)(const uint8_t *data, uint16_t length);

    void *user_ctx;
    const ModbusUserFunctionHandler *user_functions;
    uint8_t user_function_count;

    const ModbusDeviceIdObject *device_id_objects;
    uint8_t device_id_object_count;

    ModbusReadCoilsCb                   read_coils;
    ModbusReadDiscreteInputsCb          read_discrete_inputs;
    ModbusReadHoldingRegistersCb        read_holding_registers;
    ModbusReadInputRegistersCb          read_input_registers;

    ModbusWriteSingleCoilCb             write_single_coil;
    ModbusWriteSingleRegisterCb         write_single_register;
    ModbusWriteMultipleCoilsCb          write_multiple_coils;
    ModbusWriteMultipleRegistersCb      write_multiple_registers;

    ModbusMaskWriteRegisterCb           mask_write_register;
    ModbusReadWriteMultipleRegistersCb  read_write_multiple_registers;
} ModbusSlaveConfig;

/*==============================
    Slave structure
==============================*/
typedef struct {
    ModbusSlaveConfig config;
    volatile ModbusState state;

#ifdef MODBUS_DOUBLE_BUFFER
    /* Two physical buffers; frame and _rx_frame always point to different ones */
    uint8_t          _frames[2][MODBUS_MAX_FRAME_LENGTH];
    uint8_t         *frame;         /* processing buffer (poll-side) */
    uint8_t         *_rx_frame;     /* receive buffer   (ISR-side)   */
    volatile uint16_t _rx_frame_len;
    volatile bool     _rx_frame_ok;
#else
    uint8_t           frame[MODBUS_MAX_FRAME_LENGTH];
    volatile bool     processing_frame; /* guards ISR during poll */
#endif

    volatile uint16_t frame_len;    /* length of the processing buffer */
    volatile bool     frame_ok;
    volatile bool     frame_available;

    uint8_t response[MODBUS_MAX_FRAME_LENGTH]; /* avoids large stack allocation */
} ModbusSlave;

/*==============================
    Public API
==============================*/
int  modbus_slave_init(ModbusSlave *slave, const ModbusSlaveConfig *cfg);
int  modbus_slave_process_frame(ModbusSlave *slave, const uint8_t *data, uint16_t length);
void modbus_slave_rx_byte(ModbusSlave *slave, uint8_t byte);

void modbus_slave_1_5t_elapsed(ModbusSlave *slave);
void modbus_slave_3_5t_elapsed(ModbusSlave *slave);

void modbus_slave_poll(ModbusSlave *slave);

#ifdef __cplusplus
}
#endif

#endif /* MODBUS_SLAVE_H */
