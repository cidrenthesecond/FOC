add_library(NTC STATIC
    ${MAIN_DIR}/Common/Src/Thermistor.c
    NTC_Task.c 
)

target_include_directories(NTC PUBLIC
    ${MAIN_DIR}/Common/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)


