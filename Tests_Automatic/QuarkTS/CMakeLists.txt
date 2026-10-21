add_executable(QuarkTS_Test
    AllTests.c
    QuarkTS_Test.c
    QuarkTS_TestRunner.c
)

target_include_directories(QuarkTS_Test PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(QuarkTS_Test PRIVATE
    quarkts
    unity_lib
)

add_custom_command(TARGET QuarkTS_Test POST_BUILD

    COMMAND QuarkTS_Test -v

    COMMENT "Running QuarkTS tests..."
)