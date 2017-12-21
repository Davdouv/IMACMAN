#pragma once
#include "Cell.hpp"
#include <string>
#include <fstream>

class Map {

private:
    static const int m_nbX = 10;
    static const int m_nbY = 10;
    Cell m_cells[Map::m_nbX][Map::m_nbY];
    std::string m_fileMap;

public:

    // constructor 
    Map(std::string);
    Map();

    // getters
    std::string getFileMap() const;

    // setters
    void setFileMap(std::string);

    // loads a file and converts it to a Cell matrix
    int load();

    int loadJSON();

    // displays the map
    void display();
};