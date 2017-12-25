#pragma once
#include "Object.hpp"

class Character : public Object {

private:
    int m_speed;

public:

    // constructor 
    Character(int, int, int, int, int, Orientation);
    Character();

    // getter
    int getSpeed() const;

    // setter
    void setSpeed(int);

    // move methods
    virtual void move();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();


};