file(GLOB EXAMPLE_SOURCES "*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
	# todo: change link by variable 
    target_link_libraries(${EXAMPLE_NAME} PRIVATE unity sound view music core ui event ncurses pthread m)

endforeach()