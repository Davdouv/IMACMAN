#include "../include/project/StaticObject.hpp"


StaticObject::StaticObject(char type, float posX, float posY, float width, float height, Orientation orientation) : Object(type, posX, posY, width, height, orientation) { }
StaticObject::StaticObject() { }


void StaticObject::display() {
    display();
    std::cout << "StaticObject" << std::endl;
}