add_library(CMD_Transport STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Cmd_Transport.c
)

target_include_directories(CMD_Transport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    PRIVATE
        ${BSP_INCLUDE_DIR}
)

target_link_libraries(CMD_Transport 
    PRIVATE
        stm32cubemx
)