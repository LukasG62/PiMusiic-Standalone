file(GLOB PIMUSIIC_SOUND_IO_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_io STATIC ${PIMUSIIC_SOUND_IO_SOURCES})
add_library(PiMusiic::sound_io ALIAS pimusiic_sound_io)

target_include_directories(pimusiic_sound_io PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

