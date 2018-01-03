cmake_minimum_required(VERSION 2.8)

project(ImacGL)

## Definitions

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

## Packages

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

## Directories

include_directories(${SDL_INCLUDE_DIR} ${SDL_TTF_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIR} ${SDL_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} glimac/include third-party/include)

set(ALL_LIBRARIES glimac ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${SDL_MIXER_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${FREETYPE_LIBRARIES} ${HARFBUZZ_LIBRARIES})

set(CMAKE_CXX_FLAGS "-O2 -std=c++11")

add_subdirectory(glimac)

file(GLOB IMACMAN_DIRECTORIES "IMACMAN*")

foreach(IMACMAN ${IMACMAN_DIRECTORIES})
    add_subdirectory(${IMACMAN})
endforeach()
