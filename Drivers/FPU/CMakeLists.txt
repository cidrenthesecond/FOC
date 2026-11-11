add_library(FPU STATIC
    Src/FPU.c
)

target_include_directories(FPU PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/InC
)

target_link_libraries(FPU
    PRIVATE
        stm32cubemx
)