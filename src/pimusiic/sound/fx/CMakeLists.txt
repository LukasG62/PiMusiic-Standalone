file(GLOB PIMUSIIC_SOUND_FX_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_fx STATIC ${PIMUSIIC_SOUND_FX_SOURCES})
add_library(PiMusiic::sound_fx ALIAS pimusiic_sound_fx)

target_include_directories(pimusiic_sound_fx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

