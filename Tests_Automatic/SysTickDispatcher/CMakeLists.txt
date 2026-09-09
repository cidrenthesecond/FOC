add_executable(SysTickDispatcher_test)

target_sources(SysTickDispatcher_test PRIVATE
    AllTests.c
    SysTickDispatcher_TestRunner.c
    SysTickDispatcher_Test.c

    ${MAIN_DIR}/Common/Src/SysTickDispatcher.c
)

target_include_directories(SysTickDispatcher_test PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(SysTickDispatcher_test
    unity_lib
)

add_custom_command(TARGET SysTickDispatcher_test POST_BUILD
    COMMENT "Running Sys tick dispatcher tests..."
    COMMAND SysTickDispatcher_test -v
)