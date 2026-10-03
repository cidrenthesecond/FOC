add_library(FPU STATIC
    FPU.c
)

target_include_directories(FPU PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FPU
    PRIVATE
        stm32cubemx
)