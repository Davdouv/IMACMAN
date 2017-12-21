#pragma once
#include "Utilities.hpp"

class Cell {

public:

    // construcor
    Cell(int, int, char);
    Cell();
    
    // getters
    int getPosX() const;
    int getPosY() const;
    char getStaticElement() const;

    // setters
    void setPosX(int);
    void setPosY(int);
    void setStaticElement(char);

    // displays a cell 
    void display();

private:

    int m_posX;
    int m_posY;
    char m_staticElement;

};