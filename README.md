# IMACMAN

IMACMAN is a 3D game made in .cpp with OpenGL3 by a group of 3 IMAC Students.
It's basically Pacman Game, but in 3D

## HOW TO GET THE SOURCE CODE

Download the .zip folder or clone the project with following instruction :

```sh
$ git clone https://github.com/Davdouv/IMACMAN.git
```

## REQUIRED LIBRARIES

Make sure that you have SDL, SDL_ttf, SDL_image and SDL_mixer installed.
If your on Debian or Ubuntu, you can install packages:

```sh
$ sudo apt-get install libsdl-image1.2 libsdl-image1.2-dev libsdl-ttf2.0-0 libsdl-ttf2.0-dev libsdl-mixer1.2 libsdl-mixer1.2-dev
```

## COMPILATION

This projects works on Linux OS. Here are the instructions for the compilation in your bash.

To compile, create a "Build" folder next to the "Code" folder.

Open the console inside of this new folder and write :


```sh
$ cmake ../Code && make
$ ./IMACMAN_src/IMACMAN_NAME_OF_SRC_FILE
```


## CODING

The main file is in the folder Code/IMACMAN_src
All other files that you include in your main must be placed in :
- glimac/src/project for .cpp files
- glimac/include/project for .hpp files
- assets for all assets files (.png, .mp3, .obj etc...)


## Tech

* [OpenGL](https://www.opengl.org)
* [SDL](https://www.libsdl.org)

## Language

- C++
- GLSL


## Authors

We are 3 IMAC students (French Engineering School of Multimedia) in 2nd year (2017) :
- Laure ISSA (https://github.com/laureis)
- David NASR (https://github.com/Davdouv)
- Daphné ROSE (https://github.com/Damagae)
