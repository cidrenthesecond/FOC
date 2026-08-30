add_executable(WaveGen_test
    AllTests.c
    WaveGen_Test.c
    WaveGen_TestRunner.c

    ${MAIN_DIR}/common/Src/WaveGen.c
    ${MAIN_DIR}/common/Src/WaveGen_LUTs.c
    ${MAIN_DIR}/common/Src/SIN_LUT.c
    ${MAIN_DIR}/common/Src/SIGMOID_LUT.c
)

target_link_libraries(WaveGen_test PRIVATE
    unity_lib
)

target_include_directories(WaveGen_test PRIVATE
    ${MAIN_DIR}/common/Inc
)

add_executable(WaveGen_plots
    main.c
    ${MAIN_DIR}/common/Src/WaveGen.c
    ${MAIN_DIR}/common/Src/WaveGen_LUTs.c
    ${MAIN_DIR}/common/Src/SIN_LUT.c
    ${MAIN_DIR}/common/Src/SIGMOID_LUT.c
)

target_include_directories(WaveGen_plots PRIVATE
    ${MAIN_DIR}/common/Inc
)

add_custom_command(TARGET WaveGen_test POST_BUILD
    COMMENT "Running WaveGen tests..."
    COMMAND WaveGen_test -v
)

add_custom_command(TARGET WaveGen_plots POST_BUILD
    COMMAND WaveGen_Plots -v
)