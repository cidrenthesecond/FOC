add_library(Relay STATIC
    Src/Relay.c
)

target_include_directories(Relay
    PUBLIC
        inc
)

target_link_libraries(Relay
    PRIVATE
        GPIO_Interface
        ADC_Interface
        Logger_Interface
)