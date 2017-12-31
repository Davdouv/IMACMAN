#pragma once
#include "StaticObject.hpp"

class Edible : public StaticObject {

public:
  enum Type {PAC_GOMME, SUPER_PAC_GOMME, FRUIT};
  enum Fruit { NONE, APPLE, BANANA, CHERRY };
private:
  int m_type;
  int m_available;
  Fruit m_fruit;
  // Type m_type;

public:

    // constructor
    Edible(int, int, float, float, int, bool, Fruit, Orientation);
    Edible();

    // getter
    int getTypeEdible() const;
    bool getAvailability() const;
    Fruit getFruit() const;

    // setter
    void setTypeEdible(int);
    void setAvailability(bool);
    void setFruit(Fruit);
    void upgradeFruit();
    void display() override;

    // the bonus given by the edible depending on its type
    int gain();

};
