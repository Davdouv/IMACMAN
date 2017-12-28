#pragma once

#include "Character.hpp"

class Ghost : public Character {

private:
    int m_type;
    int m_death; 

public:
    /* 4 GHOSTS :
     * Shadow : follows Pacman everywhere
     * Speedy : aims the location Pacman is going to 
     * Bashful : from time to time goes the opposite way Pacman goes
     * Pokey : from time to time changes direction
     */
    enum Type {SHADOW = 0, SPEEDY = 1, BASHFUL = 2, POKEY = 3};

    // constructor
    Ghost(int, int, float, float, float, int, Orientation, int);
    Ghost();

    // getter
    int getType() const;
    int getDeath() const;

    // setter
    void setType(int);
    void setDeath(int);

    void reset() override;

    void display() override;
    void move() override;

    void slowDown();
    void randomAI();
};