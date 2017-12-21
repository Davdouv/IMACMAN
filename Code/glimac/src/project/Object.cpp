#include "../include/project/Object.hpp"

Object::Object(int width, int height) : m_width(width), m_height(height) {}
Object::Object() {}

int Object::getWidth() const { return m_width; }
int Object::getHeight() const { return m_height; }
void Object::setWidth(int width) { m_width = width; }
void Object::setHeight(int height) { m_height = height; }