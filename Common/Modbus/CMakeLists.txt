add_library(Modbus_Rtu STATIC
    Src/Rtu.c
    Src/CRC16.c
)

target_include_directories(Modbus_Rtu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)