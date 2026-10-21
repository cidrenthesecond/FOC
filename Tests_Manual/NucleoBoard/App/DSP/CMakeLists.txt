add_library(DSP STATIC
    Moving_Avarage_Filter.c
)

target_include_directories(DSP PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)