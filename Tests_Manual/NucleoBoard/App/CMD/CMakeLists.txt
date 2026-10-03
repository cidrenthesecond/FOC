add_library(commandLine STATIC
    ${MAIN_DIR}/Common/Src/CommandExecute.c
    Src/CMD_Commands.c
    Src/CMD_Task.c
    Src/Cmd_Transport.c
)

target_include_directories(commandLine PUBLIC
    ${MAIN_DIR}/Common/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    
    ${MAIN_TEST_DIR}/TODO
)

target_link_libraries(commandLine
    PRIVATE
        stm32cubemx
)