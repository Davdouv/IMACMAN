#include "../include/project/Cell.hpp"
#include <iostream>

Cell::Cell(int x, int y, char e) : m_posX(x), m_posY(y), m_staticElement(e) {}
Cell::Cell() { }

int Cell::getPosX() const { return m_posX; }
int Cell::getPosY() const { return m_posY; }
char Cell::getStaticElement() const { return m_staticElement; }

void Cell::setPosX(int x) { m_posX = x; }
void Cell::setPosY(int y) { m_posY = y; }
void Cell::setStaticElement(char e) { m_staticElement = e; }

void Cell::display() {
    std::cout << m_staticElement;
}