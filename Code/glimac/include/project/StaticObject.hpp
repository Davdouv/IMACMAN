#pragma once
#include "Object.hpp"

class StaticObject : public Object {

public:

    // constructor 
    StaticObject(char, int, int, float, float, Orientation);
    StaticObject();

    void display() override;
};