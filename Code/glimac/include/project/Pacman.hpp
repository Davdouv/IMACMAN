#pragma once

#include "Character.hpp"

class Pacman : public Character {

private:

public:

    Pacman(float, float, float, float, float, Orientation);
    Pacman();

    void display() override;
};