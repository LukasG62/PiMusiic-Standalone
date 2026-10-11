set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests/pimusiic/event")

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.test.c")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_BASENAME ${TEST_SOURCE} NAME_WE)
    set(TEST_NAME test_pimusiic_${TEST_BASENAME})
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE unity PiMusiic::event Shared::Lib)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
