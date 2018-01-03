#include "../include/project/Object.hpp"
#include "../include/project/Map.hpp"
#include "../include/project/Wall.hpp"
#include "../include/project/Edible.hpp"
#include <string>
#include <iostream>
#include <random>

using namespace glimac;

Map::Map() { }

Map::~Map()
{
    for(int i = 0; i < m_staticObjects.size(); ++i) {
        for (int j = 0; j < m_staticObjects[i].size(); ++j) {
            delete (m_staticObjects[i][j]);
        }
    }
}

//int Map::getState() const { return m_state;}
int Map::getNbX() const { return m_nbX; }
int Map::getNbY() const { return m_nbY; }
std::string Map::getFileMap() const { return m_fileMap; }
Pacman* Map::getPacman() { return &m_pacman; }
std::vector<Ghost*> Map::getGhosts() {
    std::vector<Ghost*> ghosts;
    for(unsigned int i = 0; i < m_ghosts.size(); i++)
        ghosts.push_back(&m_ghosts[i]);
    return ghosts;
}
std::vector<std::vector<StaticObject*>> Map::getStaticObjects() const { return m_staticObjects; }

//void Map::setState(int state) { m_state = state;}
void Map::setNbX(int x) { m_nbX = x; }
void Map::setNbY(int y) { m_nbY = y; }
void Map::setFileMap(std::string fileName) { m_fileMap = fileName; }
void Map::setPacman(Pacman pacman) { m_pacman = pacman; }
void Map::setGhosts(std::vector<Ghost> tabGhosts) { m_ghosts = tabGhosts; }
void Map::setStaticObjects(std::vector<std::vector<StaticObject*>> tabObjects) { m_staticObjects = tabObjects;}

// object getter

std::vector<Wall*> Map::getWalls() {

    std::vector<Wall*> walls;
    for (int i = 0; i < m_nbX; i++) {
        for (int j = 0; j < m_nbY; j++) {
            if (m_staticObjects[i][j]->getType() == 'W') walls.push_back((Wall*)m_staticObjects[i][j]);
        }
    }
    return walls;
}

std::vector<Door*> Map::getDoors() {

    std::vector<Door*> doors;
    for (int i = 0; i < m_nbX; i++) {
        for (int j = 0; j < m_nbY; j++) {
            if (m_staticObjects[i][j]->getType() == 'D')
            {
                doors.push_back((Door*)m_staticObjects[i][j]);
            }
        }
    }
    return doors;
}

std::vector<Edible*> Map::getSuperPacGommes() {

    std::vector<Edible*> superpacgomme;
    for (int i = 0; i < m_nbX; i++) {
        for (int j = 0; j < m_nbY; j++) {
            if (m_staticObjects[i][j]->getType() == 'E') {
                Edible *e;
                e = (Edible*)m_staticObjects[i][j];
                if (e->getTypeEdible() == Edible::Type::SUPER_PAC_GOMME)
                superpacgomme.push_back(e);
            }
        }
    }
    return superpacgomme;
}

std::vector<Edible*> Map::getPacGommes() {

    std::vector<Edible*> pacgomme;
    for (int i = 0; i < m_nbX; i++) {
        for (int j = 0; j < m_nbY; j++) {
            if (m_staticObjects[i][j]->getType() == 'E') {
                Edible *e;
                e = (Edible*)m_staticObjects[i][j];
                if (e->getTypeEdible() == Edible::Type::PAC_GOMME)
                pacgomme.push_back(e);
            }
        }
    }
    return pacgomme;
}

std::vector<Edible*> Map::getFruits() {

    std::vector<Edible*> fruits;
    for (int i = 0; i < m_nbX; i++) {
        for (int j = 0; j < m_nbY; j++) {
            if (m_staticObjects[i][j]->getType() == 'E') {
                Edible *e;
                e = (Edible*)m_staticObjects[i][j];
                if (e->getTypeEdible() == Edible::Type::FRUIT)
                fruits.push_back(e);
            }
        }
    }
    return fruits;
}

void Map::initialization() {
    initDoors();
}

void Map::initDoors() {

    std::vector<Door*> doors = getDoors();
    if (doors.size() == 2) {
        doors[0]->setDestX(doors[1]->getPosX());
        doors[0]->setDestY(doors[1]->getPosY());
        doors[1]->setDestX(doors[0]->getPosX());
        doors[1]->setDestY(doors[0]->getPosY());
        m_staticObjects[doors[0]->getPosY()][doors[0]->getPosX()] = doors[0];
        m_staticObjects[doors[1]->getPosY()][doors[1]->getPosX()] = doors[1];
    }
}


// For console only
void Map::display() {
    bool ghost = false;
    m_pacman.display();
    if (m_staticObjects.empty()) std::cout << "It's empty!" << std::endl;
    else {
        for (int i = 0; i < m_nbX; i++) {
            std::vector<StaticObject*> tmp = m_staticObjects.at(i);
            for (int  j = 0; j < m_nbY; j++) {
                ghost = false;
                for (int k = 0; k < m_ghosts.size(); k++) {
                    if ( (( m_ghosts[k].getPosX() >= tmp.at(j)->getPosX()) && (m_ghosts[k].getPosX() < tmp.at(j)->getPosX()+1)) && ((m_ghosts[k].getPosY() >= tmp.at(j)->getPosY()) && (m_ghosts[k].getPosY() < tmp.at(j)->getPosY()+1)) )
                 {std::cout << k+1 << " ";
                    ghost = true;}
                }
                if ( ((m_pacman.getPosX() >= tmp.at(j)->getPosX()) && (m_pacman.getPosX() < tmp.at(j)->getPosX()+1)) && ((m_pacman.getPosY() >= tmp.at(j)->getPosY()) && (m_pacman.getPosY() < tmp.at(j)->getPosY()+1)) )
                std::cout << m_pacman.getType() << " ";
                else if ((tmp.at(j) != NULL) && (!ghost)) {
                    std::cout << tmp.at(j)->getType() << " ";
                }
                else if (!ghost) std::cout << "V ";
            }
            std::cout << std::endl;
        }
    }
}
