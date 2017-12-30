#include "../include/project/Character.hpp"


Character::Character(char type, float posX, float posY, float width, float height, double speed, Orientation orientation, bool pacman) :
    Object(type, posX, posY, width, height, orientation), m_speed(speed), m_initX(posX), m_initY(posY), m_pacman(pacman) { }
Character::Character() { }

double Character::getSpeed() const { return m_speed; }
float Character::getInitX() const { return m_initX; }
float Character::getInitY() const { return m_initY; }
bool Character::isPacman() const { return m_pacman; }

void Character::setSpeed(double speed) { m_speed = speed;}
void Character::setInitX(float initX) { m_initX = initX;}
void Character::setInitY(float initY) { m_initY = initY;}
void Character::setIsPacman(bool pacman) { m_pacman = pacman;}

void Character::move() {

}

void Character::moveLeft() {
    setPosX(getPosX() - m_speed); 
    setOrientation(Object::Orientation::LEFT);   
}

void Character::moveRight() {
    setPosX(getPosX() + m_speed);    
    setOrientation(Object::Orientation::RIGHT);
}

void Character::moveUp() {
    setPosY(getPosY() - m_speed);
    setOrientation(Object::Orientation::UP);
}

void Character::moveDown() {
    setPosY(getPosY() + m_speed);
    setOrientation(Object::Orientation::DOWN);
}

void Character::display() {
    std::cout << "Character" << std::endl;
}

void Character::reset() {

    setPosX(m_initX);
    setPosY(m_initY);   
}