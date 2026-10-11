file(GLOB PIMUSIIC_VIEWS_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_views STATIC ${PIMUSIIC_VIEWS_SOURCES})
add_library(PiMusiic::views ALIAS pimusiic_views)

target_include_directories(pimusiic_views PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

