
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests")

file(GLOB TEST_SOURCES "*.c")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WLE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
	# todo: change link by variable 
    target_link_libraries(${TEST_NAME} PRIVATE unity view music core ui event ncurses pthread)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()