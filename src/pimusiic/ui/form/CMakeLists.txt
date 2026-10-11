file(GLOB PIMUSIIC_UI_FORM_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_ui_form STATIC ${PIMUSIIC_UI_FORM_SOURCES})
add_library(PiMusiic::ui_form ALIAS pimusiic_ui_form)

target_include_directories(pimusiic_ui_form PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

