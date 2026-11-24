#include "unity_fixture.h"
#include "modbus_slave.h"
#include "test_helpers.h"

#include <string.h>

TEST_GROUP(modbus_handler_read_device_identification);

static ModbusSlave       slave;
static ModbusSlaveConfig config;

static const ModbusDeviceIdObject device_id_objects[] = {
    { MODBUS_DEVICE_ID_VENDOR_NAME,          "OpenModbus" },
    { MODBUS_DEVICE_ID_PRODUCT_CODE,         "LIB" },
    { MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION, "1.0" },
    { MODBUS_DEVICE_ID_VENDOR_URL,           "https://example" },
    { 0x80u,                                 "Private" },
};

static uint16_t object_offset(uint8_t object_index)
{
    uint16_t offset = 8u; /* Address + 7-byte response PDU header. */

    for (uint8_t i = 0u; i < object_index; i++) {
        offset = (uint16_t)(offset + 2u + g_tx_buf[offset + 1u]);
    }

    return offset;
}

static void assert_object(uint8_t object_index, uint8_t object_id, const char *value)
{
    const uint16_t offset = object_offset(object_index);
    const uint8_t value_len = (uint8_t)strlen(value);

    TEST_ASSERT_EQUAL_HEX8(object_id, g_tx_buf[offset]);
    TEST_ASSERT_EQUAL_UINT8(value_len, g_tx_buf[offset + 1u]);
    TEST_ASSERT_EQUAL_MEMORY(value, &g_tx_buf[offset + 2u], value_len);
}

TEST_SETUP(modbus_handler_read_device_identification)
{
    setup_slave(&slave, &config);
    config.device_id_objects = device_id_objects;
    config.device_id_object_count = sizeof(device_id_objects) / sizeof(device_id_objects[0]);
    slave.config = config;
}

TEST_TEAR_DOWN(modbus_handler_read_device_identification) {}

TEST(modbus_handler_read_device_identification, test_read_basic_device_id)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_BASIC,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_HEX8(TEST_SLAVE_ADDR, g_tx_buf[0]);
    TEST_ASSERT_EQUAL_HEX8(MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT, g_tx_buf[1]);
    TEST_ASSERT_EQUAL_HEX8(MODBUS_MEI_TYPE_READ_DEVICE_ID, g_tx_buf[2]);
    TEST_ASSERT_EQUAL_HEX8(MODBUS_READ_DEVICE_ID_BASIC, g_tx_buf[3]);
    TEST_ASSERT_EQUAL_HEX8(MODBUS_DEVICE_ID_CONFORMITY_EXTENDED_INDIVIDUAL, g_tx_buf[4]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[5]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[6]);
    TEST_ASSERT_EQUAL_UINT8(3u, g_tx_buf[7]);

    assert_object(0u, MODBUS_DEVICE_ID_VENDOR_NAME, "OpenModbus");
    assert_object(1u, MODBUS_DEVICE_ID_PRODUCT_CODE, "LIB");
    assert_object(2u, MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION, "1.0");
}

TEST(modbus_handler_read_device_identification, test_read_regular_starts_at_requested_object)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_REGULAR,
        MODBUS_DEVICE_ID_VENDOR_URL
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[5]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[6]);
    TEST_ASSERT_EQUAL_UINT8(1u, g_tx_buf[7]);
    assert_object(0u, MODBUS_DEVICE_ID_VENDOR_URL, "https://example");
}

TEST(modbus_handler_read_device_identification, test_read_extended_includes_private_objects)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_EXTENDED,
        0x80u
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_UINT8(1u, g_tx_buf[7]);
    assert_object(0u, 0x80u, "Private");
}

TEST(modbus_handler_read_device_identification, test_read_individual_object)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_INDIVIDUAL,
        MODBUS_DEVICE_ID_PRODUCT_CODE
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_HEX8(MODBUS_READ_DEVICE_ID_INDIVIDUAL, g_tx_buf[3]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[5]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, g_tx_buf[6]);
    TEST_ASSERT_EQUAL_UINT8(1u, g_tx_buf[7]);
    assert_object(0u, MODBUS_DEVICE_ID_PRODUCT_CODE, "LIB");
}

TEST(modbus_handler_read_device_identification, test_unknown_stream_object_restarts_from_zero)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_BASIC,
        0x55u
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_UINT8(3u, g_tx_buf[7]);
    assert_object(0u, MODBUS_DEVICE_ID_VENDOR_NAME, "OpenModbus");
}

TEST(modbus_handler_read_device_identification, test_unknown_individual_object_returns_illegal_address)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_INDIVIDUAL,
        0x55u
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    assert_exception_response(MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT, MODBUS_EX_ILLEGAL_DATA_ADDRESS);
}

TEST(modbus_handler_read_device_identification, test_invalid_read_device_id_code_returns_illegal_value)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        0x05u,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    assert_exception_response(MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT, MODBUS_EX_ILLEGAL_DATA_VALUE);
}

TEST(modbus_handler_read_device_identification, test_unsupported_mei_type_returns_illegal_function)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        0x0Du,
        MODBUS_READ_DEVICE_ID_BASIC,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    assert_exception_response(MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT, MODBUS_EX_ILLEGAL_FUNCTION);
}

TEST(modbus_handler_read_device_identification, test_unconfigured_device_id_returns_illegal_function)
{
    slave.config.device_id_objects = NULL;
    slave.config.device_id_object_count = 0u;

    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_BASIC,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    assert_exception_response(MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT, MODBUS_EX_ILLEGAL_FUNCTION);
}

TEST(modbus_handler_read_device_identification, test_broadcast_read_device_id_is_ignored)
{
    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_BASIC,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, 0x00u, pdu, sizeof(pdu));

    TEST_ASSERT_FALSE(g_tx_called);
}

TEST(modbus_handler_read_device_identification, test_stream_response_uses_more_follows_when_objects_do_not_fit)
{
    static char long_a[121];
    static char long_b[121];
    static char short_c[11];
    static ModbusDeviceIdObject large_objects[3];

    memset(long_a, 'A', sizeof(long_a) - 1u);
    memset(long_b, 'B', sizeof(long_b) - 1u);
    memset(short_c, 'C', sizeof(short_c) - 1u);
    long_a[sizeof(long_a) - 1u] = '\0';
    long_b[sizeof(long_b) - 1u] = '\0';
    short_c[sizeof(short_c) - 1u] = '\0';

    large_objects[0].object_id = MODBUS_DEVICE_ID_VENDOR_NAME;
    large_objects[0].value = long_a;
    large_objects[1].object_id = MODBUS_DEVICE_ID_PRODUCT_CODE;
    large_objects[1].value = long_b;
    large_objects[2].object_id = MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION;
    large_objects[2].value = short_c;

    slave.config.device_id_objects = large_objects;
    slave.config.device_id_object_count = sizeof(large_objects) / sizeof(large_objects[0]);

    uint8_t pdu[] = {
        MODBUS_FC_ENCAPSULATED_INTERFACE_TRANSPORT,
        MODBUS_MEI_TYPE_READ_DEVICE_ID,
        MODBUS_READ_DEVICE_ID_BASIC,
        MODBUS_DEVICE_ID_VENDOR_NAME
    };

    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_TRUE(response_crc_valid());
    TEST_ASSERT_EQUAL_HEX8(0xFFu, g_tx_buf[5]);
    TEST_ASSERT_EQUAL_HEX8(MODBUS_DEVICE_ID_MAJOR_MINOR_REVISION, g_tx_buf[6]);
    TEST_ASSERT_EQUAL_UINT8(2u, g_tx_buf[7]);
    assert_object(0u, MODBUS_DEVICE_ID_VENDOR_NAME, long_a);
    assert_object(1u, MODBUS_DEVICE_ID_PRODUCT_CODE, long_b);
}
