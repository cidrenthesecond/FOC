add_library(App_init STATIC
    Src/App_Init.c
)

target_include_directories(App_init
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(App_init
    PRIVATE
        Logger
        LoggerTransport
        CMD_Task
        PWM_Driver
        ADC_Interface
        ADC_Service
        FPU
        Relay
        EmergencyHandler
        NTC_Task
        ControlSchemes
        Control_Task
        SpeedEstimation_Task
        SysTickWrapper
        PWM_Schemes
)