#pragma once
#include "StaticObject.hpp"

class Door : public StaticObject {

private:
  float m_destX;
  float m_destY;

public:
  
  // constructors
  Door(float posX, float posY, float width = 1.0, float height = 1.0, Orientation orientation = RIGHT);
  Door();

  // getters
  float getDestX() const;
  float getDestY() const;

  // setters
  void setDestX(float);
  void setDestY(float);

};