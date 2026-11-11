add_library(EmergencyHandler STATIC
    Src/EmergencyHandler.c
)

target_include_directories(EmergencyHandler
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(EmergencyHandler
    PRIVATE
        PWM_Driver
        ADC_Interface
        ADC_Service
)