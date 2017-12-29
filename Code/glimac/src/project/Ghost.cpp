#include "../include/project/Ghost.hpp"


Ghost::Ghost(int posX, int posY, float width, float height, float speed, int type2, Orientation orientation, int death) : Character('G', posX, posY, width, height, speed, orientation, false), m_type(type2), m_death(death) { }
Ghost::Ghost() { }

int Ghost::getType() const { return m_type; }
int Ghost::getDeath() const { return m_death; }

void Ghost::setType(int type) { m_type = type; }
void Ghost::setDeath(int death) { m_death = death; }

void Ghost::display() {
    std::cout << "Ghost" << std::endl;
}

bool Ghost::ready() {
    switch (m_type) {

        case SHADOW: return (SDL_GetTicks() - m_death > 1000);
            break;
        case SPEEDY: return (SDL_GetTicks() - m_death > 2000);
            break;
        case BASHFUL: return (SDL_GetTicks() - m_death > 3000);
            break;
        case POKEY: return (SDL_GetTicks() - m_death > 4000);
            break;
    }
}

void Ghost::resetLife() {

    m_death = SDL_GetTicks();
}

void Ghost::move() {

}


void Ghost::slowDown() {
    setSpeed(getSpeed()/1.5);
}

void Ghost::randomAI() {

}
