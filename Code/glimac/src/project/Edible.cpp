#include "../include/project/Edible.hpp"
#include <SDL/SDL.h>

Edible::Edible(int posX, int posY, float width, float height, int type2, bool available, Fruit fruit, Orientation orientation) : StaticObject('E', posX, posY, width, height, orientation), m_available(available),  m_fruit(fruit), m_type(type2) { }
Edible::Edible() { }

int Edible::getTypeEdible() const { return m_type; }
bool Edible::getAvailability() const { return m_available; }
Edible::Fruit Edible::getFruit() const { return m_fruit; }
void Edible::setTypeEdible(int type) { m_type = type;}
void Edible::setAvailability(bool available) { m_available = available; }
void Edible::setFruit(Edible::Fruit fruit) { m_fruit = fruit; }


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
        case FRUIT : return 400+(m_fruit*100);
            break;
        default:return 0;
    }
}

void Edible::upgradeFruit() {

    switch (getFruit()) { 
        case Edible::Fruit::CHERRY : setFruit(Edible::Fruit::NONE);
            break;
        case Edible::Fruit::APPLE : setFruit(Edible::Fruit::BANANA);
            break;
        case Edible::Fruit::BANANA : setFruit(Edible::Fruit::CHERRY);
            break;
        default:return;
    }
    setAvailability(false);
}