#pragma once
#include "Cell.hpp"

class Map {

private:
    static const int m_nbX = 10;
    static const int m_nbY = 10;
    Cell cells[Map::m_nbX][Map::m_nbY];
    char * fileMap;

public:

    Map();

};