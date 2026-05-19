file(GLOB EVENT_SOURCES "*.c")
add_library(event STATIC ${EVENT_SOURCES})
target_include_directories(event PUBLIC ${CMAKE_SOURCE_DIR}/include)