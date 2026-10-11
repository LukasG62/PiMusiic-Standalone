file(GLOB PIMUSIIC_EVENT_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_event STATIC ${PIMUSIIC_EVENT_SOURCES})
add_library(PiMusiic::event ALIAS pimusiic_event)

target_include_directories(pimusiic_event PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(pimusiic_event PUBLIC
    Shared::event
)
