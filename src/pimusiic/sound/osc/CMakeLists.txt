file(GLOB PIMUSIIC_SOUND_OSC_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_osc STATIC ${PIMUSIIC_SOUND_OSC_SOURCES})
add_library(PiMusiic::sound_osc ALIAS pimusiic_sound_osc)

target_include_directories(pimusiic_sound_osc PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

