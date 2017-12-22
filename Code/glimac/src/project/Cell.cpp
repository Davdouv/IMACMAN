#include "../include/project/Cell.hpp"
#include <iostream>

Cell::Cell(char e) :m_staticElement(e) {}
Cell::Cell() { }

char Cell::getStaticElement() const { return m_staticElement; }

void Cell::setStaticElement(char e) { m_staticElement = e; }

void Cell::display() {
    std::cout << m_staticElement;
}