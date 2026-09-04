add_executable(ScalarProfile_ManualTest)

target_sources(ScalarProfile_ManualTest PRIVATE
    main.c
    ${COMMON_SOURCE_DIR}/ScalarProfile.c
)

target_include_directories(ScalarProfile_ManualTest PRIVATE
    ${COMMON_INCLUDE_DIR}
)

add_custom_command( TARGET ScalarProfile_ManualTest POST_BUILD
    COMMENT "Running ScalarProfile manual tests..."
    COMMAND ScalarProfile_ManualTest -v
)