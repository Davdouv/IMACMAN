#include "project/Wall.hpp"

// Constructors
Wall::Wall(int posX, int posY, float width, float height/*, Orientation orientation*/) :
    StaticObject(posX, posY, width, height/*, orientation*/) { }
Wall::Wall() { }