add_library(waveGen STATIC
    ${MAIN_DIR}/Common/Src/WaveGen.c
    ${MAIN_DIR}/Common/Src/WaveGen_LUTs.c
    ${MAIN_DIR}/Common/Src/SIN_LUT.c
    ${MAIN_DIR}/Common/Src/COS_LUT.c
    ${MAIN_DIR}/Common/Src/SIGMOID_LUT.c
)

target_include_directories(waveGen PUBLIC
    ${MAIN_DIR}/Common/Inc
)
