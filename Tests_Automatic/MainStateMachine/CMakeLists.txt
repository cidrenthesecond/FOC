add_executable(MainSM_Test
    AllTests.c
    MainSM_Tests.c
    MainSM_TestsRunner.c
    "${MAIN_DIR}/Tests_Manual/NucleoBoard/TODO/MainSM.c"
    
)

target_include_directories(MainSM_Test PRIVATE
    ${MAIN_DIR}/Common/Inc
    ${MAIN_DIR}/Tests_Manual/NucleoBoard/TODO
    ${MAIN_DIR}/Tests_Manual/NucleoBoard/Schemes
)

add_subdirectory(X:/Projects/FOC_Driver/Firmware/QuarkTS-7.3.3 quarkts_build)

target_link_libraries(MainSM_Test PRIVATE
    quarkts
    unity_lib
)

add_custom_command(TARGET MainSM_Test POST_BUILD

    COMMAND MainSM_Test -v

    COMMENT "Running MainSM_Test tests..."
)