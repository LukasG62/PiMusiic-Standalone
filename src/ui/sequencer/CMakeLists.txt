file(GLOB UI_SEQUENCER_SOURCES "*.c")
target_sources(ui PRIVATE ${UI_SEQUENCER_SOURCES})
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/include)

# todo: sortir le dossier sequencer de ui et faire une lib dédiée au sequencer qui importera music et ui.
target_link_libraries(ui PRIVATE music)