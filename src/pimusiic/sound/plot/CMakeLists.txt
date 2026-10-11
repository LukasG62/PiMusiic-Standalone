file(GLOB PIMUSIIC_SOUND_PLOT_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_sound_plot STATIC ${PIMUSIIC_SOUND_PLOT_SOURCES})
add_library(PiMusiic::sound_plot ALIAS pimusiic_sound_plot)

target_include_directories(pimusiic_sound_plot PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

