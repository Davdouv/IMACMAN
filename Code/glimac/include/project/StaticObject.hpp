#pragma once
#include "Object.hpp"

class StaticObject : public Object {

public:

    // constructor 
    StaticObject(int, int, float, float, Orientation);
    StaticObject();

    void display() override;
};