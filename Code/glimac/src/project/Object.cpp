#include "../include/project/Object.hpp"

Object::Object(int width, int height) : m_width(width), m_height(height) {}
Object::Object() {}

int Object::getWidth() const { return m_width; }
int Object::getHeight() const { return m_height; }
int Object::getPosX() const { return m_posX; }
int Object::getPosY() const { return m_posY; }

void Object::setWidth(int width) { m_width = width; }
void Object::setHeight(int height) { m_height = height; }
void Object::setPosX(int x) { m_posX = x; }
void Object::setPosY(int y) { m_posY = y; }