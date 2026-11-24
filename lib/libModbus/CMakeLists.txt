add_library(libModbus STATIC
    src/modbus_crc16.c
    src/modbus_slave_handlers.c
    src/modbus_slave.c
)

target_include_directories(libModbus
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)