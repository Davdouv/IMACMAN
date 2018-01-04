#pragma once

#include "Character.hpp"
#include "../glimac/SDLWindowManager.hpp"

/* -------------
 *  GHOST CLASS
 * ------------- */

class Ghost : public Character {

private:
    int m_type;
    int m_death;
    bool m_super;
    glm::vec3 m_color;

public:
    /* 4 GHOSTS :
     * Shadow : follows Pacman everywhere
     * Speedy : aims the location Pacman is going to 
     * Bashful : from time to time goes the opposite way Pacman goes
     * Pokey : from time to time changes direction
     */
    enum Type {SHADOW = 0, SPEEDY = 1, BASHFUL = 2, POKEY = 3};

    // constructor
    Ghost(int, int, float, float, double, int, Orientation, int);
    Ghost();

    // getter
    int getType() const;
    int getDeath() const;
    bool getSuper() const;
    glm::vec3 getColor() const;

    // setter
    void setType(int);
    void setDeath(int);
    void setSuper(bool);

    bool ready();
    void reset() override;
};