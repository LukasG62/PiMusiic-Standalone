file(GLOB PIMUSIIC_CORE_SOURCES CONFIGURE_DEPENDS "*.c")

add_library(pimusiic_core STATIC ${PIMUSIIC_CORE_SOURCES})
add_library(PiMusiic::core ALIAS pimusiic_core)

target_include_directories(pimusiic_core PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
