add_executable(timer_test)

target_sources(timer_test PRIVATE
    AllTests.c
    Timer_TestRunner.c
    Timer_Test.c

    ${MAIN_DIR}/Common/Src/Timer.c
)

target_include_directories(timer_test PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(timer_test
    unity_lib
)

add_custom_command(TARGET timer_test POST_BUILD
    COMMENT "Running timer tests..."
    COMMAND timer_test -v
)