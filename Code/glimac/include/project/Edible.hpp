#pragma once
#include "StaticObject.hpp"

class Edible : public StaticObject {

public:
  enum Type {PAC_GOMME, SUPER_PAC_GOMME, FRUIT};

private:
  int m_type;
  int m_available;
  // Type m_type;

public:

    // constructor
    Edible(int, int, float, float, int, bool, Orientation);
    Edible();

    // getter
    int getTypeEdible() const;
    bool getAvailability() const;

    // setter
    void setTypeEdible(int);
    void setAvailability(bool);

    void display() override;

    // the bonus given by the edible depending on its type
    int gain();

};
