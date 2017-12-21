 #include "../include/project/Cell.hpp"


Cell::Cell(int x, int y, int e) : m_posX(x), m_posY(y), m_staticElement(e) {}

int Cell::getPosX() const { return m_posX; }
int Cell::getPosY() const { return m_posY; }
int Cell::getStaticElement() const { return m_staticElement; }

void Cell::setPosX(int x) { m_posX = x; }
void Cell::setPosY(int y) { m_posY = y; }
void Cell::setStaticElement(int e) { m_staticElement = e; }