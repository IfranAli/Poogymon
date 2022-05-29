cmake_minimum_required(VERSION 3.9)
project(game)

#set(CMAKE_CXX_FLAGS "-g -Wall")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib)

add_compile_options(-g -Wall -Wextra -Wpedantic)

find_package(PkgConfig)

pkg_check_modules(SDL2 sdl2 SDL2_ttf SDL2_image)
pkg_check_modules(SQLITE3 sqlite3)

link_directories(${SDL2_LIBRARY_DIRS})
include_directories("/opt/homebrew/lib" ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "./src/*")

add_subdirectory(src)
add_subdirectory(tests)

add_custom_target(DependencyGraph
	COMMAND cmake --graphviz=DependencyGraph.dot .
	COMMAND dot -Tsvg DependencyGraph.dot -o DependencyGraph.svg
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
