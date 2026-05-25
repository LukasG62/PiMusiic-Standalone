
add_library(sound STATIC "")

file(GLOB SOUND_ROOT_SOURCES "*.c")
target_sources(ui PRIVATE ${SOUND_ROOT_SOURCES})
target_include_directories(sound PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(env)
add_subdirectory(osc)
add_subdirectory(fx)
add_subdirectory(plot)
add_subdirectory(engine)

