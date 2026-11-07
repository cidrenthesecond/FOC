add_executable(ConfigManager_test 
    AllTests.c
    ConfigManager_TestsRunner.c
    ConfigManager_Tests.c
    FakeConfig.c
)

add_subdirectory(${MAIN_DIR}/Common/ConfigManager ConfigManager_Build)

target_include_directories(ConfigManager_test PRIVATE
    ${MAIN_DIR}/Common/ConfigManager ConfigManager_Build
)

target_link_libraries(ConfigManager_test PRIVATE
    ConfigManager
    unity_lib
)

add_custom_command(TARGET ConfigManager_test POST_BUILD

    COMMAND ConfigManager_test -v

    COMMENT "Running Config Manager tests..."

)