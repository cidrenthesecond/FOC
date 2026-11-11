add_executable(ConfigManager_test 
    AllTests.c
    ConfigManager_TestsRunner.c
    ConfigManager_Tests.c
)

target_link_libraries(ConfigManager_test PRIVATE
    ConfigManager
    test_harness
    FakeConfig
)

add_custom_command(TARGET ConfigManager_test POST_BUILD

    COMMAND ConfigManager_test -v

    COMMENT "Running Config Manager tests..."

)