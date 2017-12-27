#pragma once

#include "Character.hpp"

class Ghost : public Character {

private:
    int m_type;

public: 
    enum Type {SHADOW, SPEEDY, BASHFUL, POKEY;
    // constructor
    Ghost(int, int, float, float, int, int, Orientation);
    Ghost();

    // getter
    int getType() const;

    // setter
    void setType(int);
    void reset();

    void display() override;
};