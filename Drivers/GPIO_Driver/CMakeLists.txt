add_library(GPIO_Driver STATIC
    Src/GPIO_Driver.c
)

target_include_directories(GPIO_Driver PRIVATE
   ${BSP_INCLUDE_DIR}
)

target_link_libraries(GPIO_Driver
    PUBLIC
        GPIO_Interface
    PRIVATE
        stm32cubemx
)