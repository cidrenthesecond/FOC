add_library(PWM_Driver STATIC
    Src/PWM_Driver.c
)

target_include_directories(PWM_Driver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${BSP_INCLUDE_DIR}
)

target_link_libraries(PWM_Driver
    PRIVATE
        stm32cubemx
        Logger
)