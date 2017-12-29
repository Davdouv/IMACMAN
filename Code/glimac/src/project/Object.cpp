#include "../include/project/Object.hpp"

Object::Object(char type, float posX, float posY, float width, float height, Orientation orientation) : 
m_type(type), m_posX(posX), m_posY(posY), m_width(width), m_height(height), m_orientation(orientation) {}
Object::Object() {}

char Object::getType() const { return m_type; }
float Object::getWidth() const { return m_width; }
float Object::getHeight() const { return m_height; }
float Object::getPosX() const { return m_posX; }
float Object::getPosY() const { return m_posY; }
Object::Orientation Object::getOrientation() const { return m_orientation; }

void Object::setType(char type) { m_type = type; }
void Object::setWidth(float width) { m_width = width; }
void Object::setHeight(float height) { m_height = height; }
void Object::setPosX(float x) { m_posX = x; }
void Object::setPosY(float y) { m_posY = y; }
void Object::setOrientation(Orientation orientation) { m_orientation = orientation; }

bool Object::collision(Object *c) {
    if (c==NULL) return false;
    //float seuil = 0.005;
    float x1 = this->getPosX();
    float y1 = this->getPosY();
    float w1 = this->getWidth();
    float h1 = this->getHeight();
    float x2 = c->getPosX();
    float y2 = c->getPosY();
    float w2 = c->getWidth();
    float h2 = c->getHeight();

    if ( ((x1+w1 >= x2) && (x1 < x2+w2))
     && ((y1+h1 >= y2) && (y1 < y2+h2)) )  {
        return true;
    }
    return false; 
} 

void Object::display() {

    std::cout << "Objet : ";
}

