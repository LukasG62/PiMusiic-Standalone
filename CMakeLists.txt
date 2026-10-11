cmake_minimum_required(VERSION 3.13)
project(PiMusiic VERSION 2.0 LANGUAGES C)
enable_testing()

# Variables de configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Options de compilation
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(ADD_DEBUG_SYMBOLS "Add debug symbols to the build" ON)

add_compile_options(-Wall -Wextra)
add_compile_definitions(_DEFAULT_SOURCE _POSIX_C_SOURCE=200809L)

if(ADD_DEBUG_SYMBOLS)
	add_compile_options(-g -DDEBUG)
endif()

# Dépendances externes
find_package(Curses REQUIRED)
if(Curses_FOUND AND NOT TARGET Curses::Curses)
    add_library(Curses::Curses INTERFACE IMPORTED)
    set_target_properties(
        Curses::Curses
        PROPERTIES
            INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
    )
endif()
find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)

# Structure du projet
add_subdirectory(src)
add_subdirectory(external)

if(BUILD_TESTS)
	add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

if(BUILD_DOCS)
	add_subdirectory(docs)
endif()