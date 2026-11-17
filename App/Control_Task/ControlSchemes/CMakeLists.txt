add_library(ControlUtility
    ControlUtility.c
)

target_include_directories(ControlUtility
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ControlUtility
    PUBLIC
        WaveGen
)


add_library(ControlSchemes STATIC
    ScalarControl.c
    #ScalarControlSensored.c
)

target_include_directories(ControlSchemes 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ControlSchemes
    PRIVATE
        ADC_Interface
        ADC_Service
        ScalarProfile
        WaveGen
        PWM_Driver
        PID
        PWM_Schemes
        quarkts
        Logger
        SystemParameters
        ControlUtility
)