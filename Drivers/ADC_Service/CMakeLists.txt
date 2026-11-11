# Drivers/ADC_Service/CMakeLists.txt
add_library(ADC_Service STATIC
    Src/ADC_Service.c
)

#target_include_directories(ADC_Service PRIVATE
    #${BSP_INCLUDE_DIR}
#)

target_link_libraries(ADC_Service
    PUBLIC
        ADC_Interface
    PRIVATE
        stm32cubemx
        DSP
        Logger
)