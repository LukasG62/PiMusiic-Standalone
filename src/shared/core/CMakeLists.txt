file(GLOB SHARED_CORE_SOURCE CONFIGURE_DEPENDS "*.c")

add_library(shared_core STATIC ${SHARED_CORE_SOURCE})
add_library(Shared::core ALIAS shared_core)

target_include_directories(shared_core PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)