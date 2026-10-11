add_executable(test_AppManager
    AllTests.c
    AppManager_Tests.c
    AppManager_TestsRunner.c
    ${MAIN_DIR}/Common/Src/App_Manager.c
)

target_include_directories(test_AppManager PRIVATE
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(test_AppManager PRIVATE
    unity_lib
)

add_custom_command(TARGET test_AppManager POST_BUILD
    COMMAND test_AppManager -v
    COMMENT "Running AppManager tests..."
)