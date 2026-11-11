add_library(LoggerTransport STATIC
    Src/Log_Transport.c
)

target_include_directories(LoggerTransport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${BSP_INCLUDE_DIR}
)

target_link_libraries(LoggerTransport
    PRIVATE
        Logger
        stm32cubemx
)