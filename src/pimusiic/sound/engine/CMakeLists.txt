file(GLOB PIMUSIIC_SOUND_ENGINE_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_engine STATIC ${PIMUSIIC_SOUND_ENGINE_SOURCES})
add_library(PiMusiic::sound_engine ALIAS pimusiic_sound_engine)

target_include_directories(pimusiic_sound_engine PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

