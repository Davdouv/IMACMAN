#pragma once
#include <iostream>

class Object {

public:
    enum Orientation {LEFT = 180, RIGHT = 0, UP = 90, DOWN = 270};

private:
    char m_type;
    float m_width;
    float m_height;
    int m_posX;
    int m_posY;
    Orientation m_orientation;

public:

    // constructor 
    Object(char, int, int, float = 1.0, float = 1.0, Orientation = LEFT);
    Object();

    // getter
    char getType() const;
    float getWidth() const;
    float getHeight() const;
    int getPosX() const;
    int getPosY() const;
    Orientation getOrientation() const;

    // setter
    void setType(char);
    void setWidth(float);
    void setHeight(float);
    void setPosX(int);
    void setPosY(int);
    void setOrientation(Orientation);

    // collision
    bool collision(Object*);

    // display 
    virtual void display();
};