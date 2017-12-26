#include "../include/project/Ghost.hpp"


Ghost::Ghost(int posX, int posY, int width, int height, int speed, int type2, Orientation orientation) : Character('G', posX, posY, width, height, speed, orientation), m_type(type2) { }
Ghost::Ghost() { }

int Ghost::getType() const { return m_type; }

void Ghost::setType(int type) { m_type = type;}
void Ghost::reset() { }


void Ghost::display() {
    std::cout << "Ghost" << std::endl;
}