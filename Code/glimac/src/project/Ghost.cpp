#include "../include/project/Ghost.hpp"


Ghost::Ghost(int posX, int posY, float width, float height, double speed, int type2, Orientation orientation, int death) : Character('G', posX, posY, width, height, speed, orientation, false), m_type(type2), m_death(death), m_super(false) { }
Ghost::Ghost() { }

int Ghost::getType() const { return m_type; }
int Ghost::getDeath() const { return m_death; }
bool Ghost::getSuper() const { return m_super; }

void Ghost::setType(int type) { m_type = type; }
void Ghost::setDeath(int death) { m_death = death; }
void Ghost::setSuper(bool super) { m_super = super; }

void Ghost::display() {
    std::cout << "G";
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

void Ghost::reset () {
    setPosX(getInitX());
    setPosY(getInitY());
    if (m_super) setSpeed(getSpeed()*1.5);  
    m_death = SDL_GetTicks();
    m_super = false;
    
}

void Ghost::move() {

}

// We don't use this function with speed = speed * deltaTime
void Ghost::slowDown() {
    setSpeed(getSpeed()/1.5);
}

void Ghost::randomAI() {

}
