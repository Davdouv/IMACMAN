#pragma once
#include "Utilities.hpp"
#include "Object.hpp"
#include <vector>

class Cell {

public:

    // construcor
    Cell(char);
    Cell();

    // getters
    char getStaticElement() const;
    const std::vector<Object*> &getElements() const;

    // setters
    void setStaticElement(char);
    void setElements(const std::vector<Object*>& elements);

    // displays a cell 
    void display();

private:

    int m_posX;
    int m_posY;
    char m_staticElement;
    std::vector<Object*> m_elements;

};