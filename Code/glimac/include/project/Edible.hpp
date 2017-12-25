#pragma once
#include "StaticObject.hpp"

class Edible : public StaticObject {

public:
  enum Type {PAC_GOMME, SUPER_PAC_GOMME, FRUIT};

private:
  int m_type;
  // Type m_type;

public:

    // constructor
    Edible(int, int, float, float, int, Orientation);
    Edible();

    // getter
    int getType() const;

    // setter
    void setType(int);
};
