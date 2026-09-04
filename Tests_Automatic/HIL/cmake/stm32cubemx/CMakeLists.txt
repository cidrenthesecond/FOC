cmake_minimum_required(VERSION 3.22)

enable_language(C ASM)

# ============================================================
# STM32CubeMX defines
# ============================================================

set(MX_Defines_Syms
    USE_FULL_LL_DRIVER
    HSE_VALUE=25000000
    HSE_STARTUP_TIMEOUT=100
    LSE_STARTUP_TIMEOUT=5000
    LSE_VALUE=32768
    EXTERNAL_CLOCK_VALUE=12288000
    HSI_VALUE=64000000
    LSI_VALUE=32000
    VDD_VALUE=3300
    STM32H533xx
    $<$<CONFIG:Debug>:DEBUG>
)

# ============================================================
# STM32CubeMX include directories
# ============================================================

set(MX_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Device/ST/STM32H5xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/CMSIS/Include
)

# ============================================================
# CubeMX application sources
#
# main.c CELOWO NIE JEST TUTAJ.
# ============================================================

set(MX_Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/icache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/stm32h5xx_it.c
)

# ============================================================
# STM32 system + LL drivers
# ============================================================

set(STM32_Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/system_stm32h5xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_icache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_dma.c
)

# ============================================================
# STM32CubeMX library
# ============================================================

add_library(stm32cubemx STATIC
    ${MX_Application_Src}
    ${STM32_Drivers_Src}

    # Startup
    ${CMAKE_CURRENT_SOURCE_DIR}/../../startup_stm32h533xx.s
)

target_include_directories(stm32cubemx PUBLIC
    ${MX_Include_Dirs}
)

target_compile_definitions(stm32cubemx PUBLIC
    ${MX_Defines_Syms}
)

# ============================================================
# Linker
# ============================================================

# Jeżeli masz tutaj dodatkowe biblioteki z toolchaina,
# możesz je dodać:
#
# target_link_libraries(stm32cubemx PUBLIC
#     ${TOOLCHAIN_LINK_LIBRARIES}
# )