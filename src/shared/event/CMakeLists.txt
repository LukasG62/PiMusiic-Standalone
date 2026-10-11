file(GLOB SHARED_EVENT_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(shared_event STATIC ${SHARED_EVENT_SOURCES})
add_library(Shared::event ALIAS shared_event)

target_include_directories(shared_event PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
