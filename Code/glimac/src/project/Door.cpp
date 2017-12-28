#include "project/Door.hpp"

Door::Door(float posX, float posY, float width, float height, Orientation orientation) :
    StaticObject('D', posX, posY, width, height, orientation) { }
Door::Door() { }

float Door::getDestX() const { return m_destX; }
float Door::getDestY() const { return m_destY; }

void Door::setDestX(float destX) { m_destX = destX; }
void Door::setDestY(float destY) { m_destY = destY; }