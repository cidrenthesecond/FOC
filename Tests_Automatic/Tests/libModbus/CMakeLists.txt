add_executable(libModbusTest
    test_modbus_bytes.c
    test_modbus_crc16.c
    test_modbus_handler_mask_write_register.c
    test_modbus_handler_read_coils.c
    test_modbus_handler_read_device_identification.c
    test_modbus_handler_read_discrete_inputs.c
    test_modbus_handler_read_holding_registers.c
    test_modbus_handler_read_input_registers.c
    test_modbus_handler_read_write_multiple_registers.c
    test_modbus_handler_write_multiple_coils.c    
    test_modbus_handler_write_multiple_registers.c    
    test_modbus_handler_write_single_coil.c    
    test_modbus_handler_write_single_register.c    
    test_modbus_integration.c
    test_modbus_slave_init.c
    test_modbus_slave_rx.c
    test_modbus_user_function.c
    test_runner.c
)

target_include_directories(libModbusTest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libModbusTest
    PRIVATE
        libModbus
        test_harness
)

add_custom_command(TARGET libModbusTest POST_BUILD

    COMMAND libModbusTest -v

    COMMENT "Running libModbus tests..."

)