#include "unity_fixture.h"
#include "modbus_slave.h"
#include "test_helpers.h"

#include <string.h>

TEST_GROUP(modbus_user_function);

#define USER_FC        0x41u
#define USER_FC_OTHER  0x42u

static ModbusSlave       slave;
static ModbusSlaveConfig config;

static bool handler_called;
static uint8_t seen_function_code;
static uint8_t seen_request[16];
static uint16_t seen_request_len;
static uint16_t seen_response_max;
static void *seen_user_ctx;

static uint8_t user_ctx_value;
static ModbusExceptionCode handler_result;
static uint8_t handler_response[16];
static uint16_t handler_response_len;
static bool force_oversize_response;
static bool handler_send_response;

static ModbusExceptionCode mock_user_function(uint8_t function_code,
                                              const uint8_t *request,
                                              uint16_t request_len,
                                              uint8_t *response,
                                              uint16_t response_max,
                                              uint16_t *response_len,
                                              bool *send_response,
                                              void *user_ctx)
{
    handler_called = true;
    seen_function_code = function_code;
    seen_request_len = request_len;
    seen_response_max = response_max;
    seen_user_ctx = user_ctx;

    if (request_len <= sizeof(seen_request)) {
        memcpy(seen_request, request, request_len);
    }

    if (handler_result != MODBUS_EX_NONE) return handler_result;

    *send_response = handler_send_response;
    if (!handler_send_response) return MODBUS_EX_NONE;

    if (force_oversize_response) {
        *response_len = (uint16_t)(response_max + 1u);
        return MODBUS_EX_NONE;
    }

    if (handler_response_len > response_max) return MODBUS_EX_SLAVE_DEVICE_FAILURE;

    memcpy(response, handler_response, handler_response_len);
    *response_len = handler_response_len;
    return MODBUS_EX_NONE;
}

static const ModbusUserFunctionHandler user_functions[] = {
    { USER_FC, mock_user_function },
};

TEST_SETUP(modbus_user_function)
{
    setup_slave(&slave, &config);

    user_ctx_value = 0x5Au;
    config.user_ctx = &user_ctx_value;
    config.user_functions = user_functions;
    config.user_function_count = sizeof(user_functions) / sizeof(user_functions[0]);
    slave.config = config;

    handler_called = false;
    seen_function_code = 0u;
    memset(seen_request, 0, sizeof(seen_request));
    seen_request_len = 0u;
    seen_response_max = 0u;
    seen_user_ctx = NULL;

    handler_result = MODBUS_EX_NONE;
    memset(handler_response, 0, sizeof(handler_response));
    handler_response_len = 0u;
    force_oversize_response = false;
    handler_send_response = true;
}

TEST_TEAR_DOWN(modbus_user_function) {}

TEST(modbus_user_function, test_user_function_valid_response)
{
    handler_response[0] = 0x11u;
    handler_response[1] = 0x22u;
    handler_response[2] = 0x33u;
    handler_response_len = 3u;

    uint8_t pdu[] = { USER_FC, 0xAAu, 0xBBu };
    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(handler_called);
    TEST_ASSERT_EQUAL_HEX8(USER_FC, seen_function_code);
    TEST_ASSERT_EQUAL_UINT16(2u, seen_request_len);
    TEST_ASSERT_EQUAL_HEX8(0xAAu, seen_request[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBBu, seen_request[1]);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(MODBUS_MAX_PDU_LENGTH - 1u), seen_response_max);
    TEST_ASSERT_EQUAL_PTR(&user_ctx_value, seen_user_ctx);

    TEST_ASSERT_TRUE(g_tx_called);
    TEST_ASSERT_EQUAL_UINT16(7u, g_tx_len);
    TEST_ASSERT_EQUAL_HEX8(TEST_SLAVE_ADDR, g_tx_buf[0]);
    TEST_ASSERT_EQUAL_HEX8(USER_FC, g_tx_buf[1]);
    TEST_ASSERT_EQUAL_HEX8(0x11u, g_tx_buf[2]);
    TEST_ASSERT_EQUAL_HEX8(0x22u, g_tx_buf[3]);
    TEST_ASSERT_EQUAL_HEX8(0x33u, g_tx_buf[4]);
    TEST_ASSERT_TRUE(response_crc_valid());
}

TEST(modbus_user_function, test_unregistered_user_function_returns_illegal_function)
{
    uint8_t pdu[] = { USER_FC_OTHER, 0xAAu };
    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_FALSE(handler_called);
    assert_exception_response(USER_FC_OTHER, MODBUS_EX_ILLEGAL_FUNCTION);
}

TEST(modbus_user_function, test_user_function_exception_response)
{
    handler_result = MODBUS_EX_ILLEGAL_DATA_VALUE;

    uint8_t pdu[] = { USER_FC, 0xAAu };
    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(handler_called);
    assert_exception_response(USER_FC, MODBUS_EX_ILLEGAL_DATA_VALUE);
}

TEST(modbus_user_function, test_broadcast_user_function_executes_without_response)
{
    uint8_t pdu[] = { USER_FC, 0xAAu };
    send_request(&slave, 0x00u, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(handler_called);
    TEST_ASSERT_FALSE(g_tx_called);
}

TEST(modbus_user_function, test_user_function_can_suppress_response)
{
    handler_send_response = false;
    handler_response[0] = 0x11u;
    handler_response_len = 1u;

    uint8_t pdu[] = { USER_FC, 0xAAu };
    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(handler_called);
    TEST_ASSERT_FALSE(g_tx_called);
}

TEST(modbus_user_function, test_oversize_user_response_returns_device_failure)
{
    force_oversize_response = true;

    uint8_t pdu[] = { USER_FC };
    send_request(&slave, TEST_SLAVE_ADDR, pdu, sizeof(pdu));

    TEST_ASSERT_TRUE(handler_called);
    assert_exception_response(USER_FC, MODBUS_EX_SLAVE_DEVICE_FAILURE);
}
