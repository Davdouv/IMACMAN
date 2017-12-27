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
    int getTypeEdible() const;

    // setter
    void setTypeEdible(int);

    void display() override;

    // the bonus given by the edible depending on its type
    int gain();
};
