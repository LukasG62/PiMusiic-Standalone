file(GLOB INET_SOURCES "*.c")
add_library(inet STATIC ${INET_SOURCES})
target_include_directories(inet PUBLIC ${CMAKE_SOURCE_DIR}/include)