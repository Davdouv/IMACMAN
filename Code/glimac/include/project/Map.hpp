#pragma once
#include "Cell.hpp"
#include <string>
#include <fstream>
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "StaticObject.hpp"
#include "Controller.hpp"
#include "Edible.hpp"
#include "Wall.hpp"
#include "Player.hpp"
#include "Door.hpp"
using namespace glimac;

class Map {

private:
    int m_nbX;
    int m_nbY;
    //static const int m_nbX = 10;
    //static const int m_nbY = 10;
    int m_state;
    Player m_player;
    Pacman m_pacman;
    std::vector<Ghost> m_ghosts;
    std::vector<std::vector<StaticObject*>> m_staticObjects;
    std::string m_fileMap;

public:

    enum State { NORMAL, SUPER };
    // constructor 
    Map(std::string);
    Map();

    // Destructor
    ~Map();

    // getters
    //int getState() const;
    int getNbX() const;
    int getNbY() const;
    std::string getFileMap() const;
    Pacman* getPacman();
    std::vector<Ghost*> getGhosts();
    std::vector<std::vector<StaticObject*>> getStaticObjects() const;

    // get map elements
    std::vector<Edible*> getPacGommes();
    std::vector<Edible*> getSuperPacGommes();
    std::vector<Edible*> getFruits();
    std::vector<Wall*> getWalls();
    std::vector<Door*> getDoors();
    std::vector<StaticObject*> getSpawnPoint();

    // Doors
    void initDoors();
    // setters
    void setNbX(int);
    void setNbY(int);
    void setFileMap(std::string);
    void setPacman(Pacman);
    void setGhosts(std::vector<Ghost>);
    void setStaticObjects(std::vector<std::vector<StaticObject*>>);

    // map initialization
    void initialization();

    
    // displays the map
    void display();
};