file(GLOB MUSIC_SOURCES "*.c")
add_library(music STATIC ${MUSIC_SOURCES})
target_include_directories(music PUBLIC ${CMAKE_SOURCE_DIR}/include)