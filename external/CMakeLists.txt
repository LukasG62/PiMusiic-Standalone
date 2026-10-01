add_library(unity unity/unity.c)

target_include_directories(unity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/unity)

target_compile_definitions(unity PUBLIC 
    UNITY_OUTPUT_COLOR
    UNITY_INCLUDE_DOUBLE
)