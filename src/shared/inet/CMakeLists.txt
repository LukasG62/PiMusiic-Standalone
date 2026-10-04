file(GLOB SHARED_INET_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(shared_inet STATIC ${SHARED_INET_SOURCES})
add_library(Shared::inet ALIAS shared_inet)

target_include_directories(shared_inet PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)