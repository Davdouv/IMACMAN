#include "../include/project/Object.hpp"
#include "../include/project/Map.hpp"
#include "../include/project/Wall.hpp"
#include "../include/project/Edible.hpp"
#include <string>
#include <iostream>

using namespace glimac;

Map::Map() { }

int Map::getNbX() const { return m_nbX; }
int Map::getNbY() const { return m_nbY; }
std::string Map::getFileMap() const { return m_fileMap; }
Pacman* Map::getPacman() { return &m_pacman; }
std::vector<Ghost> Map::getGhosts() const { return m_ghosts; }
std::vector<std::vector<StaticObject*>> Map::getStaticObjects() const { return m_staticObjects; }

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

int Map::load() {

    std::fstream file;
    file.open(m_fileMap, std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << " error loading the file " << std::endl;
        return 0;
    }
    else {
        std::vector<std::vector<StaticObject*>> vec;
        int i = 0;
        std::string tmp;
        file.seekg(0);
        getline(file,tmp);
        Pacman *p = new Pacman(charToInt(tmp[1]), charToInt(tmp[2]), 0.5, 0.5, 0.005, Object::Orientation::LEFT);
        this->setPacman(*p);
        std::vector<Ghost> tabGhost;
        for (int i = 0; i < 4; i++) {   
            getline(file,tmp);
            Ghost *g = new Ghost(charToInt(tmp[1]), charToInt(tmp[2]), 0.5, 0.5, 1, i+1, Object::Orientation::LEFT);
            tabGhost.push_back(*g);
        }
        this->setGhosts(tabGhost);
        while (!file.eof()) {
            getline(file, tmp);
            std::vector<StaticObject*> row;
            for (int j = 0; j < tmp.size(); j++) {
                StaticObject *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                switch(tmp[j]) {

                    case 'W' : o = new Wall(j, i, 1, 1,  Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(j, i, 0.25, 0.25, Edible::Type::PAC_GOMME, Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(j, i, 0.4, 0.4, Edible::Type::SUPER_PAC_GOMME, Object::Orientation::LEFT);
                        break;
                    case 'B' : o = new Edible(j, i, 1, 1, Edible::Type::FRUIT, Object::Orientation::LEFT);
                        break;
                    case 'V' : o = new StaticObject('V', j, i, 1, 1, Object::Orientation::LEFT);
                        break;
                    default : break;
                }
                row.push_back(o);
            }
            vec.push_back(row);
            i++;
        }
        this->setStaticObjects(vec);
    }   
    file.close();
    return 1;
}

/*
void Map::movePacman(Controller* controller)
{
	Controller::Key action = controller->updatePlayerAction();

	switch (action)
	{
		case Controller::UP : m_pacman.moveUp();
			break;
		case Controller::DOWN : m_pacman.moveDown();
			break;
		case Controller::LEFT : m_pacman.moveLeft();
			break;
		case Controller::RIGHT : m_pacman.moveRight();
			break;
		default :
			break;
	}
}
/*
int Map::save() {

    std::fstream file;
    file.open(m_fileMap, std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << " error loading the file " << std::endl;
        return 0;
    }
    else {
        for (int i = 0; i < m_nbX; i++) {
            for (int j = 0; i < m_nbY; j++) {
                file << m_cells[i][j].getStaticElement();
            }
            file << std::endl;
        }
    }   
    file.close();
    return 1;
}

*/
void Map::play() {

    bool play = true;
    std::string line;
    while (play) {
        display();
        std::cout << "Your move : " << std::endl;
        getline(std::cin, line);
        if (line == "Z") {            
            if (!pacmanWallCollision('Z')) m_pacman.moveUp();
        }
        if (line == "Q") {    
            if (!pacmanWallCollision('Q')) m_pacman.moveLeft();
        }
        if (line == "S") {   
            if (!pacmanWallCollision('S')) m_pacman.moveDown();
        }
        if (line == "D") {    
            if (!pacmanWallCollision('D')) m_pacman.moveRight();
        }
        if (line == "C") play = false;
        pacmanGhostCollision();
    }
}

/*  ANCIENNE VERSION
void Map::play(Controller* controller)
{
    Controller::Key action = controller->getPlayerAction();

	switch (action)
	{
		case Controller::Z :
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()+1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveUp();
            } 
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::S :
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()-1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveDown();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::Q :
            if (m_cells[m_pacman.getPosX()+1][m_pacman.getPosY()].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveRight();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::D :
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()-1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveLeft();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		default :
			break;
	}
}
*/

// NOUVELLE VERSION
void Map::play(Controller* controller) {
    Controller::Key action = controller->getPlayerAction();

	switch (action)
	{
		case Controller::Z :
            if (!pacmanWallCollision('Z')) m_pacman.moveUp();
			break;
		case Controller::Q :
           if (!pacmanWallCollision('Q')) m_pacman.moveLeft();
			break;
		case Controller::S :
            if (!pacmanWallCollision('S')) m_pacman.moveDown();
			break;
		case Controller::D :
            if (!pacmanWallCollision('D')) m_pacman.moveRight();
			break;
		default :
			break;
	}
}

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
                    if (m_ghosts[k].collision(tmp.at(j))) {std::cout << k+1 << " ";
                    ghost = true;}
                }
                if (m_pacman.collision(tmp.at(j))) std::cout << m_pacman.getType() << " ";
                else if ((tmp.at(j) != NULL) && (!ghost)) std::cout << tmp.at(j)->getType() << " ";
                else if (!ghost) std::cout << "V ";
            }
            std::cout << std::endl;
        }
    }
}

void Map::pacmanGhostCollision() {

    for (int i = 0; i < m_ghosts.size(); i++) {
        if (m_pacman.collision(&m_ghosts[i])) {
            std::cout << "WOow that ghost took 1 life from ya " << std::endl;
            m_ghosts[i].reset();
        }
    }
}

bool Map::ghostCollision() {

    for (int i = 0; i < m_ghosts.size(); i++) {
        for (int j = 0; j < m_ghosts.size() && j!=i; j++) {
               if (m_ghosts[i].collision(&m_ghosts[j])) return true;
        }
    }
    return false;
}

bool Map::pacmanWallCollision(char direction) {
    int posX = (int)m_pacman.getPosX();
    int posY = (int)m_pacman.getPosY();
    switch(direction) {
        case 'Z': 
            if (posY == 0) return true;
            return (m_staticObjects[posY-1][posX]->getType()=='W');
        case 'Q':
            if (posX == 0) return true;
            return (m_staticObjects[posY][posX-1]->getType()=='W');
        case 'D':
            if (posX == 9) return true;
            return (m_staticObjects[posY][posX+1]->getType()=='W');
        case 'S':
            if (posY == 9) return true;
            return (m_staticObjects[posY+1][posX]->getType()=='W');
    }
    return false;
}


bool Map::ghostWallCollision() {
    return true;
}

