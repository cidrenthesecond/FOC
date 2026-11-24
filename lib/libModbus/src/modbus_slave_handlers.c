#include "modbus_slave_handlers.h"
#include <string.h>

// =============================================================================
// READ COILS (Function Code 0x01)
// =============================================================================

/**
 * Handle Read Coils request
 * Reads multiple coil (discrete output) values
 * Request: [Address][0x01][Start Address Hi][Lo][Quantity Hi][Lo]
 * Response: [Address][0x01][Byte Count][Coil Data...]
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_read_coils(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.read_coils) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t count = modbus_be16_get(&slave->frame[4]);

    if (count < 0x0001 || count > 0x07D0) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.read_coils(addr, count, &response[2]);
    if (ex != MODBUS_EX_NONE) return ex;

    response[0] = slave->frame[1];
    response[1] = (count + 7) / 8;
    *response_len += 2 + response[1];

    return MODBUS_EX_NONE;
}

// =============================================================================
// READ DISCRETE INPUTS (Function Code 0x02)
// =============================================================================

/**
 * Handle Read Discrete Inputs request
 * Reads multiple discrete input values
 * Request: [Address][0x02][Start Address Hi][Lo][Quantity Hi][Lo]
 * Response: [Address][0x02][Byte Count][Input Data...]
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_read_discrete_inputs(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.read_discrete_inputs) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t count = modbus_be16_get(&slave->frame[4]);

    if (count < 0x0001 || count > 0x07D0) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.read_discrete_inputs(addr, count, &response[2]);
    if (ex != MODBUS_EX_NONE) return ex;

    response[0] = slave->frame[1];
    response[1] = (count + 7) / 8;
    *response_len += 2 + response[1];

    return MODBUS_EX_NONE;
}

// =============================================================================
// READ HOLDING REGISTERS (Function Code 0x03)
// =============================================================================

/**
 * Handle Read Holding Registers request
 * Reads multiple 16-bit holding register values
 * Request: [Address][0x03][Start Address Hi][Lo][Quantity Hi][Lo]
 * Response: [Address][0x03][Byte Count][Register Data Hi/Lo...]
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_read_holding_registers(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.read_holding_registers) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t count = modbus_be16_get(&slave->frame[4]);

    if (count < 0x0001 || count > 0x007D) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.read_holding_registers(addr, count, &response[2]);
    if (ex != MODBUS_EX_NONE) return ex;

    response[0] = slave->frame[1];
    response[1] = count * 2;
    *response_len += 2 + response[1];

    return MODBUS_EX_NONE;
}

// =============================================================================
// READ INPUT REGISTERS (Function Code 0x04)
// =============================================================================

/**
 * Handle Read Input Registers request
 * Reads multiple 16-bit input register values
 * Request: [Address][0x04][Start Address Hi][Lo][Quantity Hi][Lo]
 * Response: [Address][0x04][Byte Count][Register Data Hi/Lo...]
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_read_input_registers(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.read_input_registers) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t count = modbus_be16_get(&slave->frame[4]);

    if (count < 0x0001 || count > 0x007D) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.read_input_registers(addr, count, &response[2]);
    if (ex != MODBUS_EX_NONE) return ex;

    response[0] = slave->frame[1];
    response[1] = count * 2;
    *response_len += 2 + response[1];

    return MODBUS_EX_NONE;
}

// =============================================================================
// WRITE SINGLE COIL (Function Code 0x05)
// =============================================================================

/**
 * Handle Write Single Coil request
 * Writes one coil (discrete output) value
 * Request: [Address][0x05][Coil Address Hi][Lo][Value Hi][Lo]
 * Response: Echo of request
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_write_single_coil(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.write_single_coil) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t value = modbus_be16_get(&slave->frame[4]);

    if (value != 0x0000 && value != 0xFF00) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.write_single_coil(addr, (value == 0xFF00) ? 1 : 0);
    if (ex != MODBUS_EX_NONE) return ex;

    memcpy(response, slave->frame + 1, 5);
    *response_len += 5;

    return MODBUS_EX_NONE;
}

// =============================================================================
// WRITE SINGLE REGISTER (Function Code 0x06)
// =============================================================================

/**
 * Handle Write Single Register request
 * Writes one 16-bit holding register
 * Request: [Address][0x06][Register Address Hi][Lo][Value Hi][Lo]
 * Response: Echo of request
 * Minimum frame length: 8 bytes
 */
ModbusExceptionCode handle_write_single_register(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 8) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.write_single_register) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr  = modbus_be16_get(&slave->frame[2]);
    uint16_t value = modbus_be16_get(&slave->frame[4]);

    ModbusExceptionCode ex = slave->config.write_single_register(addr, value);
    if (ex != MODBUS_EX_NONE) return ex;

    memcpy(response, slave->frame + 1, 5); /* FC(1) + reg_addr(2) + value(2) */
    *response_len += 5;

    return MODBUS_EX_NONE;
}

// =============================================================================
// WRITE MULTIPLE COILS (Function Code 0x0F)
// =============================================================================

/**
 * Handle Write Multiple Coils request
 * Writes multiple coil (discrete output) values
 * Request: [Address][0x0F][Start Address Hi][Lo][Quantity Hi][Lo][Byte Count][Coil Data...]
 * Response: [Address][0x0F][Start Address Hi][Lo][Quantity Hi][Lo]
 * Minimum frame length: 10 bytes (1 coil = 1 data byte)
 */
ModbusExceptionCode handle_write_multiple_coils(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 10) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.write_multiple_coils) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr       = modbus_be16_get(&slave->frame[2]);
    uint16_t count      = modbus_be16_get(&slave->frame[4]);
    uint8_t  byte_count = slave->frame[6];

    if (count < 0x0001 || count > 0x07B0)                        return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (byte_count != (count + 7) / 8)                           return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (slave->frame_len < (uint16_t)(9u + byte_count))          return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.write_multiple_coils(addr, count, &slave->frame[7]);
    if (ex != MODBUS_EX_NONE) return ex;

    memcpy(response, slave->frame + 1, 5);
    *response_len += 5;

    return MODBUS_EX_NONE;
}

// =============================================================================
// WRITE MULTIPLE REGISTERS (Function Code 0x10)
// =============================================================================

/**
 * Handle Write Multiple Registers request
 * Writes multiple 16-bit holding registers
 * Request: [Address][0x10][Start Address Hi][Lo][Quantity Hi][Lo][Byte Count][Data...]
 * Response: [Address][0x10][Start Address Hi][Lo][Quantity Hi][Lo]
 * Minimum frame length: 11 bytes (1 register = 2 data bytes)
 */
ModbusExceptionCode handle_write_multiple_registers(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 11) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.write_multiple_registers) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr       = modbus_be16_get(&slave->frame[2]);
    uint16_t count      = modbus_be16_get(&slave->frame[4]);
    uint8_t  byte_count = slave->frame[6];

    if (count < 0x0001 || count > 0x007B)                        return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (byte_count != count * 2)                                 return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (slave->frame_len < (uint16_t)(9u + byte_count))          return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.write_multiple_registers(addr, count, &slave->frame[7]);
    if (ex != MODBUS_EX_NONE) return ex;

    memcpy(response, slave->frame + 1, 5); /* FC(1) + start_addr(2) + qty(2) */
    *response_len += 5;

    return MODBUS_EX_NONE;
}

// =============================================================================
// MASK WRITE REGISTER (Function Code 0x16)
// =============================================================================

/**
 * Handle Mask Write Register request
 * Modifies specific bits in a holding register using AND/OR masks
 * Request: [Address][0x16][Register Address Hi][Lo][AND Mask Hi][Lo][OR Mask Hi][Lo]
 * Response: Echo of request
 * Minimum frame length: 10 bytes
 */
ModbusExceptionCode handle_mask_write_register(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 10) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.mask_write_register) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t addr     = modbus_be16_get(&slave->frame[2]);
    uint16_t and_mask = modbus_be16_get(&slave->frame[4]);
    uint16_t or_mask  = modbus_be16_get(&slave->frame[6]);

    ModbusExceptionCode ex = slave->config.mask_write_register(addr, and_mask, or_mask);
    if (ex != MODBUS_EX_NONE) return ex;

    memcpy(response, slave->frame + 1, 7); /* FC(1) + reg_addr(2) + and_mask(2) + or_mask(2) */
    *response_len += 7;

    return MODBUS_EX_NONE;
}

// =============================================================================
// READ/WRITE MULTIPLE REGISTERS (Function Code 0x17)
// =============================================================================

/**
 * Handle Read/Write Multiple Registers request
 * Performs a write operation followed by a read operation in a single request
 * Request: [Address][0x17][Read Address Hi][Lo][Read Quantity Hi][Lo]
 *          [Write Address Hi][Lo][Write Quantity Hi][Lo][Write Byte Count][Write Data...]
 * Response: [Address][0x17][Byte Count][Read Register Data Hi/Lo...]
 * Minimum frame length: 15 bytes (1 write register = 2 data bytes)
 */
ModbusExceptionCode handle_read_write_multiple_registers(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len < 15) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.read_write_multiple_registers) return MODBUS_EX_ILLEGAL_FUNCTION;

    uint16_t read_addr        = modbus_be16_get(&slave->frame[2]);
    uint16_t read_count       = modbus_be16_get(&slave->frame[4]);
    uint16_t write_addr       = modbus_be16_get(&slave->frame[6]);
    uint16_t write_count      = modbus_be16_get(&slave->frame[8]);
    uint8_t  write_byte_count = slave->frame[10];

    if (read_count < 0x0001  || read_count > 0x007D)             return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (write_count < 0x0001 || write_count > 0x0079)            return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (write_byte_count != write_count * 2)                     return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (slave->frame_len < (uint16_t)(13u + write_byte_count))   return MODBUS_EX_ILLEGAL_DATA_VALUE;

    ModbusExceptionCode ex = slave->config.read_write_multiple_registers(
        read_addr, read_count, write_addr, write_count,
        &slave->frame[11], &response[2]
    );
    if (ex != MODBUS_EX_NONE) return ex;

    response[0] = slave->frame[1];
    response[1] = read_count * 2;
    *response_len += 2 + response[1];

    return MODBUS_EX_NONE;
}

// =============================================================================
// READ DEVICE IDENTIFICATION (Function Code 0x2B / MEI Type 0x0E)
// =============================================================================

#define DEVICE_ID_RESPONSE_HEADER_LEN 7u
#define DEVICE_ID_OBJECT_HEADER_LEN   2u
#define DEVICE_ID_MORE_FOLLOWS_NO     0x00u
#define DEVICE_ID_MORE_FOLLOWS_YES    0xFFu

static bool device_id_read_code_is_valid(uint8_t read_code) {
    return read_code == MODBUS_READ_DEVICE_ID_BASIC      ||
           read_code == MODBUS_READ_DEVICE_ID_REGULAR    ||
           read_code == MODBUS_READ_DEVICE_ID_EXTENDED   ||
           read_code == MODBUS_READ_DEVICE_ID_INDIVIDUAL;
}

static uint8_t device_id_max_object_for_stream(uint8_t read_code) {
    if (read_code == MODBUS_READ_DEVICE_ID_BASIC) return MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION;
    if (read_code == MODBUS_READ_DEVICE_ID_REGULAR) return 0x7Fu;
    return 0xFFu;
}

static uint8_t device_id_conformity_level(const ModbusSlave *slave) {
    uint8_t max_object_id = 0u;

    for (uint8_t i = 0u; i < slave->config.device_id_object_count; i++) {
        const uint8_t object_id = slave->config.device_id_objects[i].object_id;
        if (object_id > max_object_id) max_object_id = object_id;
    }

    if (max_object_id <= MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION) {
        return MODBUS_DEVICE_ID_CONFORMITY_BASIC_INDIVIDUAL;
    }
    if (max_object_id <= 0x7Fu) {
        return MODBUS_DEVICE_ID_CONFORMITY_REGULAR_INDIVIDUAL;
    }
    return MODBUS_DEVICE_ID_CONFORMITY_EXTENDED_INDIVIDUAL;
}

static size_t device_id_value_len(const ModbusDeviceIdObject *object) {
    return object->value ? strlen(object->value) : 0u;
}

static const ModbusDeviceIdObject *device_id_find_exact(const ModbusSlave *slave, uint8_t object_id) {
    for (uint8_t i = 0u; i < slave->config.device_id_object_count; i++) {
        const ModbusDeviceIdObject *object = &slave->config.device_id_objects[i];
        if (object->object_id == object_id) return object;
    }

    return NULL;
}

static const ModbusDeviceIdObject *device_id_find_next_stream_object(const ModbusSlave *slave,
                                                                     uint16_t min_object_id,
                                                                     uint8_t max_object_id) {
    const ModbusDeviceIdObject *best = NULL;
    uint16_t best_object_id = 0x100u;

    for (uint8_t i = 0u; i < slave->config.device_id_object_count; i++) {
        const ModbusDeviceIdObject *object = &slave->config.device_id_objects[i];
        if (object->object_id < min_object_id) continue;
        if (object->object_id > max_object_id) continue;
        if (object->object_id >= best_object_id) continue;

        best = object;
        best_object_id = object->object_id;
    }

    return best;
}

static bool device_id_stream_object_exists(const ModbusSlave *slave,
                                           uint8_t object_id,
                                           uint8_t max_object_id) {
    const ModbusDeviceIdObject *object = device_id_find_exact(slave, object_id);
    return object != NULL && object->object_id <= max_object_id;
}

static ModbusExceptionCode device_id_append_object(uint8_t *response,
                                                   uint16_t *response_len,
                                                   const ModbusDeviceIdObject *object) {
    const size_t value_len = device_id_value_len(object);
    if (value_len > 0xFFu) return MODBUS_EX_SLAVE_DEVICE_FAILURE;

    if ((uint16_t)(*response_len + DEVICE_ID_OBJECT_HEADER_LEN + value_len) > MODBUS_MAX_PDU_LENGTH) {
        return MODBUS_EX_SLAVE_DEVICE_FAILURE;
    }

    response[(*response_len)++] = object->object_id;
    response[(*response_len)++] = (uint8_t)value_len;
    if (value_len > 0u) {
        memcpy(&response[*response_len], object->value, value_len);
        *response_len = (uint16_t)(*response_len + value_len);
    }

    return MODBUS_EX_NONE;
}

/**
 * Handle Read Device Identification request.
 * Request:  [Address][0x2B][0x0E][Read Device ID Code][Object ID]
 * Response: [Address][0x2B][0x0E][Read Device ID Code][Conformity]
 *           [More Follows][Next Object ID][Object Count][Objects...]
 */
ModbusExceptionCode handle_read_device_identification(ModbusSlave *slave, uint8_t *response, uint16_t *response_len) {
    if (slave->frame_len != 7u) return MODBUS_EX_ILLEGAL_DATA_VALUE;
    if (!slave->config.device_id_objects || slave->config.device_id_object_count == 0u) {
        return MODBUS_EX_ILLEGAL_FUNCTION;
    }

    const uint8_t mei_type = slave->frame[2];
    const uint8_t read_code = slave->frame[3];
    const uint8_t requested_object_id = slave->frame[4];

    if (mei_type != MODBUS_MEI_TYPE_READ_DEVICE_ID) return MODBUS_EX_ILLEGAL_FUNCTION;
    if (!device_id_read_code_is_valid(read_code)) return MODBUS_EX_ILLEGAL_DATA_VALUE;

    response[0] = MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT;
    response[1] = MODBUS_MEI_TYPE_READ_DEVICE_ID;
    response[2] = read_code;
    response[3] = device_id_conformity_level(slave);
    response[4] = DEVICE_ID_MORE_FOLLOWS_NO;
    response[5] = 0u;
    response[6] = 0u;
    *response_len = DEVICE_ID_RESPONSE_HEADER_LEN;

    if (read_code == MODBUS_READ_DEVICE_ID_INDIVIDUAL) {
        const ModbusDeviceIdObject *object = device_id_find_exact(slave, requested_object_id);
        if (object == NULL) return MODBUS_EX_ILLEGAL_DATA_ADDRESS;

        ModbusExceptionCode ex = device_id_append_object(response, response_len, object);
        if (ex != MODBUS_EX_NONE) return ex;

        response[6] = 1u;
        return MODBUS_EX_NONE;
    }

    const uint8_t max_object_id = device_id_max_object_for_stream(read_code);
    uint16_t next_object_id = requested_object_id;

    /* The spec says an unknown stream Object ID restarts from object 0. */
    if (!device_id_stream_object_exists(slave, requested_object_id, max_object_id)) {
        next_object_id = 0u;
    }

    while (next_object_id <= max_object_id) {
        const ModbusDeviceIdObject *object =
            device_id_find_next_stream_object(slave, next_object_id, max_object_id);

        if (object == NULL) break;

        const size_t value_len = device_id_value_len(object);
        if (value_len > 0xFFu) return MODBUS_EX_SLAVE_DEVICE_FAILURE;

        const uint16_t object_len = (uint16_t)(DEVICE_ID_OBJECT_HEADER_LEN + value_len);
        if ((uint16_t)(*response_len + object_len) > MODBUS_MAX_PDU_LENGTH) {
            if (response[6] == 0u) return MODBUS_EX_SLAVE_DEVICE_FAILURE;

            response[4] = DEVICE_ID_MORE_FOLLOWS_YES;
            response[5] = object->object_id;
            return MODBUS_EX_NONE;
        }

        ModbusExceptionCode ex = device_id_append_object(response, response_len, object);
        if (ex != MODBUS_EX_NONE) return ex;

        response[6]++;
        next_object_id = (uint16_t)object->object_id + 1u;
    }

    if (response[6] == 0u) return MODBUS_EX_ILLEGAL_DATA_ADDRESS;

    return MODBUS_EX_NONE;
}
