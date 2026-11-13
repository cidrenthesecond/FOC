add_library(MainSM STATIC
    Src/MainSM.c
)

target_include_directories(MainSM
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(MainSM
    PRIVATE
        quarkts
        Logger
        App_init
        NTC_Task
        Control_Task
        PWM_Driver
        RUN_LED_Task
)