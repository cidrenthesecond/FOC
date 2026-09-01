add_executable( ScalarProfile_test

    AllTests.c
    ScalarProfile_Test.c
    ScalarProfile_TestsRunner.c

    ${MAIN_DIR}/Common/Src/ScalarProfile.c
)

target_include_directories( ScalarProfile_test PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(ScalarProfile_test PRIVATE
    unity_lib
)

add_custom_command(TARGET ScalarProfile_test POST_BUILD

    COMMAND ScalarProfile_test -v
    COMMENT "Running Scalar Profile tests..."
)