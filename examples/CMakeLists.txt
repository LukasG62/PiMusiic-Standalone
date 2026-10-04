file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples")
file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink 
    "${CMAKE_CURRENT_SOURCE_DIR}/data" 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data"
)

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    
    target_link_libraries(${EXAMPLE_NAME} PRIVATE 
        PiMusiic::sound
        PiMusiic::ui
        PiMusiic::views
        PiMusiic::event
        Shared::Lib
        Curses::Curses
        ALSA::ALSA
        unity
    )
endforeach()
