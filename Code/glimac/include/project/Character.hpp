#pragma once
#include "Object.hpp"

/* -------------
 *  CHARACTER CLASS
 * ------------- */
// representing pacman and the ghosts

class Character : public Object {

private:
    float m_initX;
    float m_initY;
    float m_speed;
    bool m_pacman;
    bool m_inSpawn;

public:

    // constructor 
    Character(char, float, float, float, float, double, Orientation, bool);
    Character();

    // getter
    double getSpeed() const;
    float getInitX() const;
    float getInitY() const;
    bool isPacman() const;
    bool inSpawn() const;

    // setter
    void setSpeed(double);
    void setInitX(float);
    void setInitY(float);
    void setIsPacman(bool);
    void setInSpawn(bool);

    // move methods
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    virtual void reset();
};