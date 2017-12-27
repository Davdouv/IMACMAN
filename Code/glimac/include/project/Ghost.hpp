#pragma once

#include "Character.hpp"

class Ghost : public Character {

private:
    int m_type;

public:
    /* 4 GHOSTS :
     * Shadow : follows Pacman everywhere
     * Speedy : aims the location Pacman is going to 
     * Bashful : from time to time goes the opposite way Pacman goes
     * Pokey : from time to time changes direction
     */
    enum Type {SHADOW = 0, SPEEDY = 1, BASHFUL = 2, POKEY = 3};

    // constructor
    Ghost(int, int, float, float, float, int, Orientation);
    Ghost();

    // getter
    int getType() const;

    // setter
    void setType(int);
    void reset();

    void display() override;
    void move() override;

    void slowDown();
    void randomAI();
};