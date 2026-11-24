add_executable(Modbus_Rtu_test
    AllTests.c
    Modbus_Rtu_Test.c
    Modbus_Rtu_TestRunner.c
)

target_link_libraries(Modbus_Rtu_test
    PRIVATE
        test_harness
        Modbus_Rtu     
)

add_custom_command(TARGET Modbus_Rtu_test POST_BUILD

    COMMAND Modbus_Rtu_test -v

    COMMENT "Running Modbus RTU tests..."

)