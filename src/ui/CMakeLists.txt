
add_library(ui STATIC "") # On crée une bibliothèque vide au début

file(GLOB UI_ROOT_SOURCES "*.c")
target_sources(ui PRIVATE ${UI_ROOT_SOURCES})
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(base)
add_subdirectory(form)
add_subdirectory(sequencer)

