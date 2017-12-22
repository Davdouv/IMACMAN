#pragma once
#include "StaticObject.hpp"

class Edible : public StaticObject {

private:
  int m_type;

public:

    // constructor
    Edible(int, int, int);
    Edible();

    // getter
    int getType() const;

    // setter
    void setType(int);
};
