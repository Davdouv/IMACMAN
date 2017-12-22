#include "../include/project/Edible.hpp"


Edible::Edible(int width, int height, int type) : StaticObject(width, height), m_type(type) { }
Edible::Edible() { }

int Edible::getType() const { return m_type; }

void Edible::setType(int type) { m_type = type;}
