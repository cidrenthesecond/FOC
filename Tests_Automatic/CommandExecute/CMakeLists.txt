add_executable(CommandExecute_test)

target_sources(CommandExecute_test PRIVATE
    AllTests.c
    CommandExecute_TestsRunner.c
    CommandExecute_Test.c

    ${MAIN_DIR}/Common/Src/CommandExecute.c
)

target_include_directories(CommandExecute_test PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(CommandExecute_test PRIVATE
    unity_lib
)

add_custom_command(TARGET CommandExecute_test POST_BUILD
    COMMENT "Running CommandExecute tests..."
    COMMAND CommandExecute_test -v
)