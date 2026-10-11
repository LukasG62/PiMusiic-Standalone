file(GLOB PIMUSIIC_SOUND_ENV_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_env STATIC ${PIMUSIIC_SOUND_ENV_SOURCES})
add_library(PiMusiic::sound_env ALIAS pimusiic_sound_env)

target_include_directories(pimusiic_sound_env PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

