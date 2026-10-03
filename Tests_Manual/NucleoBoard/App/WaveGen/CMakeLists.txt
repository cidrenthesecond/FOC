add_library(waveGen STATIC
    ${MAIN_DIR}/Common/Src/Fifo.c
    ${MAIN_DIR}/Common/Src/Logger.c
    Src/Log_Transport.c
)

target_include_directories(logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${MAIN_DIR}/Common/Inc
)

target_link_libraries(logger
    PRIVATE
        stm32cubemx
)