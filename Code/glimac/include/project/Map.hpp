#pragma once
#include <iostream>

class Map {


private:
    static int m_nbX = 10;
    static int m_nbY = 10;
    int Map[m_nbX][m_nbY];

public:
    Map();
    
    // getter
    loadMap(char*);

}