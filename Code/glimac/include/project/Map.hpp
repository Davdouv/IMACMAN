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
using namespace glimac;

class Map {

private:
    //  static const int m_nbX = 36;
    //  static const int m_nbY = 30;
    static const int m_nbX = 10;
    static const int m_nbY = 10;

    Player m_player;
    Pacman m_pacman;
    std::vector<Ghost> m_ghosts;
    std::vector<std::vector<StaticObject*>> m_staticObjects;
    std::string m_fileMap;

public:

    // constructor 
    Map(std::string);
    Map();

    // Destructor
    ~Map();

    // getters
    int getNbX() const;
    int getNbY() const;
    std::string getFileMap() const;
    Pacman* getPacman();
    std::vector<Ghost> getGhosts() const;
    std::vector<std::vector<StaticObject*>> getStaticObjects() const;

    // get map elements
    std::vector<Edible*> getPacGommes();
    std::vector<Edible*> getSuperPacGommes();
    std::vector<Edible*> getFruits();
    std::vector<Wall*> getWalls();

    // setters
    void setFileMap(std::string);
    void setPacman(Pacman);
    void setGhosts(std::vector<Ghost>);
    void setStaticObjects(std::vector<std::vector<StaticObject*>>);

    // map initialization
    void initialization();

    // loads a file and converts it to a Cell matrix
    int load();
    int save();
    
    // displays the map
    void display();
    
    void play();
    void play(Controller* controller);

    //void movePacman(Controller*);

    // collisions
    void pacmanGhostCollision();
    bool ghostCollision();
    bool pacmanWallCollision(char);
    bool ghostWallCollision(int, char);
    void pacmanEdibleCollision();

    // Ghost moves AI
    void shadowAI();
    void speedyAI();
    int bashfulAI();
    void pokeyAI();
    void ghostMove();
    
    // Shortest way
    void shortestWay(int, float, float);
};