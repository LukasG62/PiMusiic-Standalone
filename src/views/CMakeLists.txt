file(GLOB VIEW_SOURCES "*.c")
add_library(view STATIC ${VIEW_SOURCES})
target_include_directories(view PUBLIC ${CMAKE_SOURCE_DIR}/include)