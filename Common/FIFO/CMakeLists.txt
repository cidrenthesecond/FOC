add_library(FIFO STATIC
    Src/FIFO.c
)

target_include_directories(FIFO 
    PUBLIC
        Inc
)