add_library(SpeedEstimation_Task STATIC
    Src/SpeedEstimation_Task.c
)

target_include_directories(SpeedEstimation_Task
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

target_link_libraries(SpeedEstimation_Task
    PRIVATE
        quarkts
)