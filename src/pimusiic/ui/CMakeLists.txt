add_library(pimusiic_ui INTERFACE)
add_library(PiMusiic::ui ALIAS pimusiic_ui)

target_include_directories(pimusiic_ui INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_subdirectory(base)
add_subdirectory(form)
add_subdirectory(sequencer)

target_link_libraries(pimusiic_ui INTERFACE
    PiMusiic::ui_base
    PiMusiic::ui_form
    PiMusiic::ui_sequencer
)

