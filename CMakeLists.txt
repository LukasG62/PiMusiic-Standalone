cmake_minimum_required(VERSION 3.12)
project(PiMusic VERSION 2.0 LANGUAGES C)
enable_testing()

cmake_policy(SET CMP0079 NEW)

# Variables de configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Options de compilation
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(BUILD_EXAMPLES "Build example applications" ON)
option(ADD_DEBUG_SYMBOLS "Add debug symbols to the build" ON)

# Dépendances externes
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)

if(ADD_DEBUG_SYMBOLS)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -DDEBUG")
endif()

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
