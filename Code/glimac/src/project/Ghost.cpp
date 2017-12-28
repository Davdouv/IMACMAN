#include "../include/project/Ghost.hpp"


Ghost::Ghost(int posX, int posY, float width, float height, float speed, int type2, Orientation orientation, int death) : Character('G', posX, posY, width, height, speed, orientation), m_type(type2), m_death(death) { }
Ghost::Ghost() { }

int Ghost::getType() const { return m_type; }
int Ghost::getDeath() const { return m_death; }

void Ghost::setType(int type) { m_type = type;}
void Ghost::setDeath(int death) { m_death = death; }

void Ghost::reset() { 
    switch (m_type) {
        case SHADOW: setDeath(20*2);
            break;
        case SPEEDY: setDeath(30*2);
            break;
        case BASHFUL: setDeath(40*2);
            break;
        case POKEY: setDeath(50*2);
            break;
    }
    if (m_death > 0) m_death--;
}

void Ghost::display() {
    std::cout << "Ghost" << std::endl;
}

void Ghost::move() {

}


void Ghost::slowDown() {
    setSpeed(getSpeed()/1.5);
}

void Ghost::randomAI() {

}
