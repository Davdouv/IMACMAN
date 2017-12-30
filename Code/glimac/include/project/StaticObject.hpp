#pragma once
#include "Object.hpp"

class StaticObject : public Object {

public:

    // constructor
    StaticObject(char, float, float, float, float, Orientation = LEFT);
    StaticObject();

    void display() override;
};
