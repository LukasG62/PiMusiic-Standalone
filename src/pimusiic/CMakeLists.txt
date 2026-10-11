file(GLOB PIMUSIIC_SOURCES CONFIGURE_DEPENDS "*.c")

add_executable(pimusiic ${PIMUSIIC_SOURCES})
add_executable(PiMusiic::App ALIAS pimusiic)

target_include_directories(pimusiic PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_subdirectory(core)
add_subdirectory(event)
add_subdirectory(sound)
add_subdirectory(ui)
add_subdirectory(views)

target_link_libraries(pimusiic PRIVATE
    PiMusiic::core
    PiMusiic::event
    PiMusiic::sound
    PiMusiic::ui
    PiMusiic::views
    Shared::Lib
    Curses::Curses
    ALSA::ALSA
)
