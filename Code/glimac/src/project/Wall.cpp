#include "project/Wall.hpp"


Wall::Wall(float posX, float posY, float width, float height, Orientation orientation) :
    StaticObject('W', posX, posY, width, height, orientation) { }
Wall::Wall() { }