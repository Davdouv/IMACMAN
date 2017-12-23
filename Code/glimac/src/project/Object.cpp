#include "../include/project/Object.hpp"

Object::Object(int posX, int posY, float width, float height, Orientation orientation) : 
m_posX(posX), m_posY(posY), m_width(width), m_height(height), m_orientation(orientation) {}
Object::Object() {}

float Object::getWidth() const { return m_width; }
float Object::getHeight() const { return m_height; }
int Object::getPosX() const { return m_posX; }
int Object::getPosY() const { return m_posY; }
Object::Orientation Object::getOrientation() const { return m_orientation; }

void Object::setWidth(float width) { m_width = width; }
void Object::setHeight(float height) { m_height = height; }
void Object::setPosX(int x) { m_posX = x; }
void Object::setPosY(int y) { m_posY = y; }
void Object::setOrientation(Orientation orientation) { m_orientation = orientation; }