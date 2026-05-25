file(GLOB SOUND_PLOT_SOURCES "*.c")
# On "donne" les fichiers au parent sans créer de bibliothèque
target_sources(sound PRIVATE ${SOUND_PLOT_SOURCES})