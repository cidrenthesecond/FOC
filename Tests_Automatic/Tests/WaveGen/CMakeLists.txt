add_executable(WaveGen_test
    AllTests.c
    WaveGen_Test.c
    WaveGen_TestRunner.c
)

target_link_libraries(WaveGen_test 
    PRIVATE
        test_harness
        WaveGen
)

add_executable(WaveGen_plots
    main.c
)

target_link_libraries(WaveGen_plots
    PRIVATE
        WaveGen
)

add_custom_command(TARGET WaveGen_test POST_BUILD
    COMMENT "Running WaveGen tests..."
    COMMAND WaveGen_test -v
)

add_custom_command(TARGET WaveGen_plots POST_BUILD
    COMMAND WaveGen_Plots -v
)