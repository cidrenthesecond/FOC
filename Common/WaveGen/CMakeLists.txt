add_library(WaveGen STATIC
    Src/WaveGen.c
    Src/WaveGen_LUTs.c
    Private/COS_LUT.c
    Private/SIGMOID_LUT.c
    Private/SIN_LUT.c
)

target_include_directories(WaveGen
    PRIVATE
        Private
    PUBLIC
        Inc
)