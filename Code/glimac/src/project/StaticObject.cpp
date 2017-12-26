#include "../include/project/StaticObject.hpp"


StaticObject::StaticObject(int posX, int posY, float width, float height, Orientation orientation) : Object(posX, posY, width, height, orientation) { }
StaticObject::StaticObject() { }


void StaticObject::display() {
    display();
    std::cout << "StaticObject" << std::endl;
}