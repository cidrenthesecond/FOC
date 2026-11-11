add_library(SysTickWrapper STATIC
    Src/SysTickWrapper.c
)

target_include_directories(SysTickWrapper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(SysTickWrapper
    PRIVATE
        stm32cubemx
)