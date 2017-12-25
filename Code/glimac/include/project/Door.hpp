#pragma once
#include "StaticObject.hpp"

class Door : public StaticObject {

private:
  int m_destination;

public:

    // constructor
    Edible(int, int, float, float, int, Orientation);
    Edible();

    // getter
    int getType() const;

    // setter
    void setType(int);
};
