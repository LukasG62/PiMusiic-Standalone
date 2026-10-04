add_library(shared_lib INTERFACE)
add_library(Shared::Lib ALIAS shared_lib)

target_include_directories(shared_lib INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_subdirectory(core)
add_subdirectory(event)
add_subdirectory(inet)
add_subdirectory(music)

target_link_libraries(shared_lib INTERFACE
    Shared::core
    Shared::event
    Shared::inet
    Shared::music
    Threads::Threads
    m
)