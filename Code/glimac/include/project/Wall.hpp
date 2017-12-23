#pragma once

#include "glimac/Geometry.hpp"
#include "StaticObject.hpp"

class Wall : public StaticObject {

private:

public:
    // Constructors
    Wall(int posX, int posY, int width = 1, int height = 1);
    Wall();
};