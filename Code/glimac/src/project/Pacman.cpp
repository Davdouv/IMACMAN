#include "../include/project/Pacman.hpp"

Pacman::Pacman(int posX, int posY, int width, int height, int speed, Orientation orientation) : Character(posX, posY, width, height, speed, orientation) { }
Pacman::Pacman() { }

void Pacman::display() {
    std::cout << "Pacman " << getPosX() << " " << getPosY() << std::endl;
}
