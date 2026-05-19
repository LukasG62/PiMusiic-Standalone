file(GLOB UI_FORM_SOURCES "*.c")
target_sources(ui PRIVATE ${UI_FORM_SOURCES})
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/include)