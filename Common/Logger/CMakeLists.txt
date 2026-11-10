add_library(Logger STATIC
    Src/Logger.c
)

target_include_directories(Logger
    PUBLIC
        Inc
)

target_link_libraries(Logger
    PRIVATE
        FIFO
)

add_library(Logger_Interface INTERFACE)

target_include_directories(Logger_Interface
    INTERFACE
        Inc
)

target_link_libraries(Logger_Interface
    INTERFACE
        FIFO
)