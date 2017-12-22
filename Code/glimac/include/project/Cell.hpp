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
    bool getAccess() const;

    // setters
    void setStaticElement(char);
    void setElements(const std::vector<Object*>& elements);
    void setAccess(bool);

    // displays a cell 
    void display();

private:

    char m_staticElement;
    std::vector<Object*> m_elements;
    bool m_access;
};