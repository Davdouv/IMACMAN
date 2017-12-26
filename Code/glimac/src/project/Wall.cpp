#include "project/Wall.hpp"

// Constructors
Wall::Wall(int posX, int posY, float width, float height, Orientation orientation) :
    StaticObject('W', posX, posY, width, height, orientation) { }
Wall::Wall() { }


void Wall::display() {
    std::cout << "Wall " << std::endl;
}