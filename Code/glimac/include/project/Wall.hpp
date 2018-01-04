#pragma once

#include "glimac/Geometry.hpp"
#include "StaticObject.hpp"

/* -------------
 *  WALL CLASS
 * ------------- */

class Wall : public StaticObject {

private:

public:
    // Constructors
    Wall(float posX, float posY, float width = 1.0, float height = 1.0, Orientation orientation = RIGHT);
    Wall();
};