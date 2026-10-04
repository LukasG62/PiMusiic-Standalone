file(GLOB PIMUSIIC_UI_BASE_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_ui_base STATIC ${PIMUSIIC_UI_BASE_SOURCES})
add_library(PiMusiic::ui_base ALIAS pimusiic_ui_base)

target_include_directories(pimusiic_ui_base PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

