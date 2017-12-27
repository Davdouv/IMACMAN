#include "../include/project/Edible.hpp"


Edible::Edible(int posX, int posY, float width, float height, int type2, Orientation orientation) : StaticObject('E', posX, posY, width, height, orientation), m_type(type2) { }
Edible::Edible() { }

int Edible::getTypeEdible() const { return m_type; }

void Edible::setTypeEdible(int type) { m_type = type;}


void Edible::display() {
    std::cout << "Edible " << m_type << std::endl;
}