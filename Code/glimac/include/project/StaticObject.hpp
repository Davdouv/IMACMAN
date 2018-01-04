#pragma once
#include "Object.hpp"

/* ---------------
 *  STATIC OBJECTS
 * --------------- */
// including walls, doors, empty paths and dots

class StaticObject : public Object {

public:

    // constructor
    StaticObject(char, float, float, float, float, Orientation = LEFT);
    StaticObject();
};
