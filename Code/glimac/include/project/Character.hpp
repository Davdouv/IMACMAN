#pragma once
#include "Object.hpp"

class Character : public Object {

private:
    float m_initX;
    float m_initY;
    float m_speed;
    bool m_pacman;

public:

    // constructor 
    Character(char, float, float, float, float, double, Orientation, bool);
    Character();

    // getter
    double getSpeed() const;
    float getInitX() const;
    float getInitY() const;
    bool isPacman() const;

    // setter
    void setSpeed(double);
    void setInitX(float);
    void setInitY(float);
    void setIsPacman(bool);

    // move methods
    virtual void move();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();


    void display() override;
    virtual void reset();
};