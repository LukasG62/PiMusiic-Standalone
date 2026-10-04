file(GLOB PI2ISERV_SOURCES CONFIGURE_DEPENDS "*.c")

add_executable(pi2iserv ${PI2ISERV_SOURCES})
add_executable(Pi2iServ::App ALIAS pi2iserv)

target_include_directories(pi2iserv PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(pi2iserv PRIVATE
    Shared::Lib
)
