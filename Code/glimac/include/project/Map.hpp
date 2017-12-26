#pragma once
#include "Cell.hpp"
#include <string>
#include <fstream>
#include "Pacman.hpp"
#include "Ghost.hpp"

class Map {

private:
    static const int m_nbX = 10;
    static const int m_nbY = 10;

    Pacman m_pacman;
    std::vector<Ghost> m_ghosts;
    Cell m_cells[Map::m_nbX][Map::m_nbY];
    std::string m_fileMap;

public:

    // constructor 
    Map(std::string);
    Map();

    // getters
    std::string getFileMap() const;
    Pacman getPacman() const;

    // setters
    void setFileMap(std::string);

    // loads a file and converts it to a Cell matrix
    int load();
    int save();
    
    // displays the map
    void display();

    void play();

    // collisions
    void pacmanGhostCollision();
    bool ghostCollision();
    bool wallCollision(Cell);

};