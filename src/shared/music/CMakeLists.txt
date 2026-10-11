file(GLOB MUSIC_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(shared_music STATIC ${MUSIC_SOURCES})
add_library(Shared::music ALIAS shared_music)

target_include_directories(shared_music PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)