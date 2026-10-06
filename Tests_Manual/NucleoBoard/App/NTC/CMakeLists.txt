add_library(NTC STATIC
    NTC_Task.c
    ${MAIN_DIR}/Common/Src/Thermistor.c
)

target_include_directories(FPU PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MAIN_DIR}/Common/Inc
)


