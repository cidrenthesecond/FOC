add_executable(CommandExecute_test)

add_compile_options(-v)

target_sources(CommandExecute_test PRIVATE
    AllTests.c
    InitAndRegister_Test.c
    InitAndRegister_TestsRunner.c
    Execute_TestsRunner.c
    Execute_Test.c
    ArgumentParsing_Test.c
    ArgumentParsing_TestRunner.c
)

target_link_libraries(CommandExecute_test 
    PRIVATE
        test_harness
        CommandExecute
)

add_custom_command(TARGET CommandExecute_test POST_BUILD
    COMMENT "Running CommandExecute tests..."
    COMMAND CommandExecute_test -v
)