#include "../include/project/Pacman.hpp"

Pacman::Pacman(float posX, float posY, float width, float height, double speed, Orientation orientation) : Character('P', posX, posY, width, height, speed, orientation, true) { }
Pacman::Pacman() { }