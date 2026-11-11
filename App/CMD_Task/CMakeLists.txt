add_library(CMD_Task STATIC
    Src/CMD_Task.c
    Src/CMD_Commands.c
)

target_include_directories(CMD_Task
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(CMD_Task
    PUBLIC
        quarkts
    PRIVATE
        quarkts
        CMD_Transport
        CommandExecute
        Logger
        ADC_Interface
        ADC_Service
        Thermistor
        ControlSchemes
        MainSM
        PWM_Driver
)