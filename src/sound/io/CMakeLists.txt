file(GLOB SOUND_IO_SOURCES "*.c")

target_sources(sound PRIVATE ${SOUND_IO_SOURCES})