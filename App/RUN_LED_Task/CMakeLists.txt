add_library(LED_Task STATIC
    Src/LED_Task.c
)

target_include_directories(LED_Task
    PUBLIC
        Inc
)

target_link_libraries(LED_Task
    PUBLIC
        quarkts
)