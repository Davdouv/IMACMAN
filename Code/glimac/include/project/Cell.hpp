#pragma once

enum staticElementType {

    EMPTY = 0,
    WALL = 1,
    GUM = 2,
    SUPER_GUM = 3
};

class Cell {

public:

    // construcor
    Cell(int, int, int);

    // getters
    int getPosX() const;
    int getPosY() const;
    int getStaticElement() const;

    // setters
    void setPosX(int);
    void setPosY(int);
    void setStaticElement(int);

private:

    int m_posX;
    int m_posY;
    int m_staticElement;

};