#include "../include/project/Object.hpp"

Object::Object(char type, int posX, int posY, float width, float height, Orientation orientation) : 
m_type(type), m_posX(posX), m_posY(posY), m_width(width), m_height(height), m_orientation(orientation) {}
Object::Object() {}

char Object::getType() const { return m_type; }
float Object::getWidth() const { return m_width; }
float Object::getHeight() const { return m_height; }
int Object::getPosX() const { return m_posX; }
int Object::getPosY() const { return m_posY; }
Object::Orientation Object::getOrientation() const { return m_orientation; }

void Object::setType(char type) { m_type = type; }
void Object::setWidth(float width) { m_width = width; }
void Object::setHeight(float height) { m_height = height; }
void Object::setPosX(int x) { m_posX = x; }
void Object::setPosY(int y) { m_posY = y; }
void Object::setOrientation(Orientation orientation) { m_orientation = orientation; }

bool Object::collision(Object *c) {
    if (c==NULL) return false;
    if ((this->getPosX() == c->getPosX()) && (this->getPosY() == c->getPosY())) {
        return true;
    }
    return false; 
    
} 

void Object::display() {

    std::cout << "Objet : ";
}

