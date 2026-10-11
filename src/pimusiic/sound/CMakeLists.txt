add_library(pimusiic_sound INTERFACE)
add_library(PiMusiic::sound ALIAS pimusiic_sound)

target_include_directories(pimusiic_sound INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_subdirectory(engine)
add_subdirectory(env)
add_subdirectory(fx)
add_subdirectory(io)
add_subdirectory(osc)
add_subdirectory(plot)

target_link_libraries(pimusiic_sound INTERFACE
    PiMusiic::sound_engine
    PiMusiic::sound_env
    PiMusiic::sound_fx
    PiMusiic::sound_io
    PiMusiic::sound_osc
    PiMusiic::sound_plot
)

