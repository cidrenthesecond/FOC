add_library(Thermistor STATIC
    Src/Thermistor.c
)

target_include_directories(Thermistor
    PUBLIC
        Inc
    PRIVATE
        Src
)

target_link_libraries(Thermistor
    PRIVATE
        ADC_Interface
)