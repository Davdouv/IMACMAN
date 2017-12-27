#pragma once
#include "StaticObject.hpp"

class Door : public StaticObject {

private:
  int m_destination;

public:

    // constructor
    Door(float, float, float, float, int, Orientation);
    Door();

    // getter
    int getType() const;

    // setter
    void setType(int);
};
