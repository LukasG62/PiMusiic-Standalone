file(GLOB SOUND_OSC_SOURCES "*.c")
# On "donne" les fichiers au parent sans créer de bibliothèque
target_sources(sound PRIVATE ${SOUND_OSC_SOURCES})