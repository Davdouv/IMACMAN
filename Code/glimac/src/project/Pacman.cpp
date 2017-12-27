#include "../include/project/Pacman.hpp"

Pacman::Pacman(int posX, int posY, float width, float height, int speed, Orientation orientation) : Character('P', posX, posY, width, height, speed, orientation) { }
Pacman::Pacman() { }

void Pacman::display() {
    std::cout << "Pacman " << getPosX() << " " << getPosY() << std::endl;
}
