file(GLOB PIMUSIIC_UI_SEQ_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_ui_sequencer STATIC ${PIMUSIIC_UI_SEQ_SOURCES})
add_library(PiMusiic::ui_sequencer ALIAS pimusiic_ui_sequencer)

target_include_directories(pimusiic_ui_sequencer PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(pimusiic_ui_sequencer PUBLIC
    Shared::music
)
