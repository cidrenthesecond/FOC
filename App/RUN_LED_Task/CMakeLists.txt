add_library(RUN_LED_Task STATIC
    Src/RUN_LED_Task.c
)

target_include_directories(RUN_LED_Task
    PUBLIC
        Inc
)

target_link_libraries(RUN_LED_Task
    PRIVATE
        quarkts
        GPIO_Interface
)