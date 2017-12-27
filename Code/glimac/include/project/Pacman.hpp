#pragma once

#include "Character.hpp"

class Pacman : public Character {

private:

public:

    Pacman(int, int, float, float, int, Orientation);
    Pacman();

    void display() override;
};