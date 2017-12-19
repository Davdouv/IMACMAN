cmake_minimum_required(VERSION 2.8)

project(ImacGL)

## Definitions

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

## Packages

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

## Directories

include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} glimac/include third-party/include)

set(ALL_LIBRARIES glimac ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

add_subdirectory(glimac)

file(GLOB IMACMAN_DIRECTORIES "IMACMAN*")

foreach(IMACMAN ${IMACMAN_DIRECTORIES})
    add_subdirectory(${IMACMAN})
endforeach()
