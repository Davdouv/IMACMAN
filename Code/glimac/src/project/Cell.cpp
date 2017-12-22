#include "../include/project/Cell.hpp"
#include <iostream>

Cell::Cell(char e) :m_staticElement(e) {}
Cell::Cell() { }

char Cell::getStaticElement() const { return m_staticElement; }
bool Cell::getAccess() const { return m_access; }

void Cell::setStaticElement(char e) { m_staticElement = e; }
void Cell::setAccess(bool access) { m_access = access; }

void Cell::display() {
    std::cout << m_staticElement;
}