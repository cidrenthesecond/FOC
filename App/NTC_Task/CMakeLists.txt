add_library(NTC_Task STATIC
    Src/NTC_Task.c
)

target_include_directories(NTC_Task
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(NTC_Task
    PUBLIC
        quarkts
    PRIVATE
        Thermistor
        Logger
        ADC_Interface
)