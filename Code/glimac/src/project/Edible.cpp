#include "../include/project/Edible.hpp"
#include <SDL/SDL.h>

Edible::Edible(int posX, int posY, float width, float height, int type2, bool available, Orientation orientation) : StaticObject('E', posX, posY, width, height, orientation), m_available(available),  m_type(type2) { }
Edible::Edible() { }

int Edible::getTypeEdible() const { return m_type; }
bool Edible::getAvailability() const { return m_available; }
void Edible::setTypeEdible(int type) { m_type = type;}
void Edible::setAvailability(bool available) { m_available = available; }


void Edible::display() {
    switch(getTypeEdible()) {

        case FRUIT: std::cout << "F ";
            break;
        case PAC_GOMME: std::cout << "G ";
            break;
        case SUPER_PAC_GOMME: std::cout << "S ";
            break;
        default:return;
    }
}

int Edible::gain() {

    switch(m_type) {

        case PAC_GOMME : return 10;
            break;
        case SUPER_PAC_GOMME : return 50;
            break;
        case FRUIT : return 400;
            break;
        default:return 0;
    }
}

