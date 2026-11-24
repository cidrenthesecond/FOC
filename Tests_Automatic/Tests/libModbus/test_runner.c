#include "unity_fixture.h"

TEST_GROUP_RUNNER(modbus_crc16) {
    RUN_TEST_CASE(modbus_crc16, test_crc16_empty_data);
    RUN_TEST_CASE(modbus_crc16, test_crc16_known_vector);
    RUN_TEST_CASE(modbus_crc16, test_crc16_another_vector);
    RUN_TEST_CASE(modbus_crc16, test_crc16_full_buffer);
    RUN_TEST_CASE(modbus_crc16, test_crc16_single_byte);
    RUN_TEST_CASE(modbus_crc16, test_crc16_table_consistency);
}

TEST_GROUP_RUNNER(modbus_bytes) {
    RUN_TEST_CASE(modbus_bytes, test_be16_get_set);
    RUN_TEST_CASE(modbus_bytes, test_be16_zero_value);
    RUN_TEST_CASE(modbus_bytes, test_be16_max_value);
    RUN_TEST_CASE(modbus_bytes, test_le16_get_set);
    RUN_TEST_CASE(modbus_bytes, test_le16_zero_value);
    RUN_TEST_CASE(modbus_bytes, test_le16_max_value);
    RUN_TEST_CASE(modbus_bytes, test_endian_consistency);
}

TEST_GROUP_RUNNER(modbus_slave_init) {
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_success);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_preserves_config);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_null_slave);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_null_config);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_null_write_function);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_address_zero);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_address_max_valid);
    RUN_TEST_CASE(modbus_slave_init, test_slave_init_address_reserved);
}

TEST_GROUP_RUNNER(modbus_slave_rx) {
    RUN_TEST_CASE(modbus_slave_rx, test_rx_first_byte_transition);
    RUN_TEST_CASE(modbus_slave_rx, test_rx_multiple_bytes);
    RUN_TEST_CASE(modbus_slave_rx, test_rx_frame_overflow);
#ifndef MODBUS_DOUBLE_BUFFER
    RUN_TEST_CASE(modbus_slave_rx, test_rx_ignore_during_processing);
#endif
    RUN_TEST_CASE(modbus_slave_rx, test_1_5t_timer_transition);
    RUN_TEST_CASE(modbus_slave_rx, test_1_5t_timer_ignore_other_states);
    RUN_TEST_CASE(modbus_slave_rx, test_3_5t_timer_valid_frame);
    RUN_TEST_CASE(modbus_slave_rx, test_3_5t_timer_invalid_frame);
    RUN_TEST_CASE(modbus_slave_rx, test_3_5t_timer_ignore_other_states);
}

TEST_GROUP_RUNNER(modbus_handler_read_coils) {
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_frame_too_short);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_valid);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_null_callback);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_count_zero);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_count_at_max);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_count_at_min);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_read_coils, test_read_coils_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_read_discrete_inputs) {
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_frame_too_short);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_valid);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_null_callback);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_count_zero);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_count_at_max);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_count_at_min);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_read_discrete_inputs, test_read_discrete_inputs_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_read_holding_registers) {
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_frame_too_short);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_valid);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_null_callback);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_count_zero);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_count_at_max);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_count_at_min);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_read_holding_registers, test_read_holding_registers_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_read_input_registers) {
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_frame_too_short);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_valid);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_null_callback);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_count_zero);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_count_at_max);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_count_at_min);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_read_input_registers, test_read_input_registers_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_write_single_coil) {
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_frame_too_short);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_on);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_off);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_null_callback);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_invalid_value);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_write_single_coil, test_write_single_coil_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_write_single_register) {
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_frame_too_short);
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_valid);
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_max_value);
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_null_callback);
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_write_single_register, test_write_single_register_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_write_multiple_coils) {
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_frame_too_short);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_valid);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_null_callback);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_count_zero);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_count_at_max);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_invalid_byte_count);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_write_multiple_coils, test_write_multiple_coils_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_write_multiple_registers) {
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_frame_too_short);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_valid);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_null_callback);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_count_zero);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_count_at_max);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_invalid_byte_count);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_write_multiple_registers, test_write_multiple_registers_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_mask_write_register) {
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_frame_too_short);
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_valid);
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_zero_masks);
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_null_callback);
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_mask_write_register, test_mask_write_register_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_read_write_multiple_registers) {
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_frame_too_short);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_valid);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_null_callback);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_read_count_zero);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_read_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_write_count_zero);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_write_count_exceeds_max);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_invalid_byte_count);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_callback_addr_error);
    RUN_TEST_CASE(modbus_handler_read_write_multiple_registers, test_read_write_multiple_registers_callback_device_failure);
}

TEST_GROUP_RUNNER(modbus_handler_read_device_identification) {
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_read_basic_device_id);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_read_regular_starts_at_requested_object);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_read_extended_includes_private_objects);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_read_individual_object);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_unknown_stream_object_restarts_from_zero);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_unknown_individual_object_returns_illegal_address);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_invalid_read_device_id_code_returns_illegal_value);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_unsupported_mei_type_returns_illegal_function);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_unconfigured_device_id_returns_illegal_function);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_broadcast_read_device_id_is_ignored);
    RUN_TEST_CASE(modbus_handler_read_device_identification, test_stream_response_uses_more_follows_when_objects_do_not_fit);
}

TEST_GROUP_RUNNER(modbus_integration) {
    RUN_TEST_CASE(modbus_integration, test_complete_frame_processing);
    RUN_TEST_CASE(modbus_integration, test_response_crc_valid);
    RUN_TEST_CASE(modbus_integration, test_process_complete_frame_without_poll);
    RUN_TEST_CASE(modbus_integration, test_frame_invalid_crc);
    RUN_TEST_CASE(modbus_integration, test_wrong_address_frame);
    RUN_TEST_CASE(modbus_integration, test_broadcast_read_ignored);
    RUN_TEST_CASE(modbus_integration, test_broadcast_write_executes_no_response);
    RUN_TEST_CASE(modbus_integration, test_unknown_function_code);
    RUN_TEST_CASE(modbus_integration, test_exception_response_crc_valid);
    RUN_TEST_CASE(modbus_integration, test_frame_len_reset_after_poll);
    RUN_TEST_CASE(modbus_integration, test_consecutive_frames);
}

TEST_GROUP_RUNNER(modbus_user_function) {
    RUN_TEST_CASE(modbus_user_function, test_user_function_valid_response);
    RUN_TEST_CASE(modbus_user_function, test_unregistered_user_function_returns_illegal_function);
    RUN_TEST_CASE(modbus_user_function, test_user_function_exception_response);
    RUN_TEST_CASE(modbus_user_function, test_broadcast_user_function_executes_without_response);
    RUN_TEST_CASE(modbus_user_function, test_user_function_can_suppress_response);
    RUN_TEST_CASE(modbus_user_function, test_oversize_user_response_returns_device_failure);
}

static void run_all_tests(void)
{
    RUN_TEST_GROUP(modbus_crc16);
    RUN_TEST_GROUP(modbus_bytes);
    RUN_TEST_GROUP(modbus_slave_init);
    RUN_TEST_GROUP(modbus_slave_rx);
    RUN_TEST_GROUP(modbus_handler_read_coils);
    RUN_TEST_GROUP(modbus_handler_read_discrete_inputs);
    RUN_TEST_GROUP(modbus_handler_read_holding_registers);
    RUN_TEST_GROUP(modbus_handler_read_input_registers);
    RUN_TEST_GROUP(modbus_handler_write_single_coil);
    RUN_TEST_GROUP(modbus_handler_write_single_register);
    RUN_TEST_GROUP(modbus_handler_write_multiple_coils);
    RUN_TEST_GROUP(modbus_handler_write_multiple_registers);
    RUN_TEST_GROUP(modbus_handler_mask_write_register);
    RUN_TEST_GROUP(modbus_handler_read_write_multiple_registers);
    RUN_TEST_GROUP(modbus_handler_read_device_identification);
    RUN_TEST_GROUP(modbus_integration);
    RUN_TEST_GROUP(modbus_user_function);
}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, run_all_tests);
}
