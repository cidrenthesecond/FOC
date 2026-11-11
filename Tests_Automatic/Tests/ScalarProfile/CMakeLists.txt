add_executable( ScalarProfile_test
    AllTests.c
    ScalarProfile_Test.c
    ScalarProfile_TestsRunner.c
)

target_link_libraries(ScalarProfile_test PRIVATE
    test_harness
    ScalarProfile
)

add_custom_command(TARGET ScalarProfile_test POST_BUILD

    COMMAND ScalarProfile_test -v
    COMMENT "Running Scalar Profile tests..."
)