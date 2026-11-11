add_library(scalarProfile STATIC
    ${MAIN_DIR}/Common/Src/scalarProfile.c
)

target_include_directories(scalarProfile PUBLIC
    ${MAIN_DIR}/Common/Inc
)
