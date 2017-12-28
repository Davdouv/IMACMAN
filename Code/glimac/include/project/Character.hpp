#pragma once
#include "Object.hpp"

class Character : public Object {

private:
    float m_initX;
    float m_initY;
    float m_speed;

public:

    // constructor 
    Character(char, float, float, float, float, float, Orientation);
    Character();

    // getter
    float getSpeed() const;
    float getInitX() const;
    float getInitY() const;

    // setter
    void setSpeed(float);
    void setInitX(float);
    void setInitY(float);

    // move methods
    virtual void move();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();


    void display() override;
    virtual void reset();
};