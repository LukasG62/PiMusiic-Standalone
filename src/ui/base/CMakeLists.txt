file(GLOB UI_BASE_SOURCES "*.c")
# On "donne" les fichiers au parent sans créer de bibliothèque
target_sources(ui PRIVATE ${UI_BASE_SOURCES})