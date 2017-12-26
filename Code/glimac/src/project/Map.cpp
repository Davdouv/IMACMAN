#include "../include/project/Object.hpp"
#include "../include/project/Map.hpp"
#include "../include/project/Wall.hpp"
#include "../include/project/Edible.hpp"
#include <string>
#include <iostream>

Map::Map() { }

std::string Map::getFileMap() const { return m_fileMap; }
Pacman Map::getPacman() const { return m_pacman; }
std::vector<Ghost> Map::getGhosts() const { return m_ghosts; }
std::vector<std::vector<StaticObject*>> Map::getStaticObjects() const { return m_staticObjects; }

void Map::setFileMap(std::string fileName) { m_fileMap = fileName; }
void Map::setPacman(Pacman pacman) { m_pacman = pacman; }
void Map::setGhosts(std::vector<Ghost> tabGhosts) { m_ghosts = tabGhosts; }
void Map::setStaticObjects(std::vector<std::vector<StaticObject*>> tabObjects) { m_staticObjects = tabObjects;}

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
        Pacman *p = new Pacman(charToInt(tmp[1]), charToInt(tmp[2]), 10, 10, 1, Object::Orientation::LEFT);
        this->setPacman(*p);
        std::vector<Ghost> tabGhost;
        for (int i = 0; i < 4; i++) {   
            getline(file,tmp);
            Ghost *g = new Ghost(charToInt(tmp[1]), charToInt(tmp[2]), 10, 10, 1, i+1, Object::Orientation::LEFT);
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

                    case 'W' : o = new Wall(j, i, 10, 10,  Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(j, i, 10, 10, 1,  Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(j, i, 10, 10, 2,  Object::Orientation::LEFT);
                        break;
                    case 'B' : o = new Edible(j, i, 10, 10, 3, Object::Orientation::LEFT);
                        break;
                    case 'V' : o = new StaticObject('V', j, i, 10, 10, Object::Orientation::LEFT);
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


void Map::movePacman(Controller* controller)
{
	Controller::Key action = controller->updatePlayerAction();

	switch (action)
	{
		case Controller::UP : 
            if (!pacmanWallCollision('Z')) m_pacman.moveUp();
    		break;
		case Controller::DOWN : 
            if (!pacmanWallCollision('S')) m_pacman.moveDown();
			break;
		case Controller::LEFT : 
            if (!pacmanWallCollision('Q')) m_pacman.moveLeft();
			break;
		case Controller::RIGHT : 
            if (!pacmanWallCollision('D')) m_pacman.moveRight();
            break;
		default :
			break;
	}
    pacmanGhostCollision();
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

/*
void Map::play(Controller* controller)
{
	Controller::Key action = controller->updatePlayerAction();

	switch (action)
	{
		case Controller::UP :
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()+1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveUp();
            } 
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::DOWN :
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()-1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveDown();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::LEFT :
            if (m_cells[m_pacman.getPosX()+1][m_pacman.getPosY()].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveRight();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
			break;
		case Controller::RIGHT :
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
void Map::display() {

    bool ghost = false;
    m_pacman.display();
    if (m_staticObjects.empty()) std::cout << "It's empty!" << std::endl;
    else {
        for (int i = 0; i < 10; i++) {
            std::vector<StaticObject*> tmp = m_staticObjects.at(i);
            for (int  j = 0; j < 10; j++) {
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
    switch(direction) {
        case 'Z': 
            if (m_pacman.getPosY() == 0) return true;
            return (m_staticObjects[m_pacman.getPosY()-1][m_pacman.getPosX()]->getType()=='W');
        case 'Q':
            if (m_pacman.getPosX() == 0) return true;
            return (m_staticObjects[m_pacman.getPosY()][m_pacman.getPosX()-1]->getType()=='W');
        case 'D':
            if (m_pacman.getPosX() == 9) return true;
            return (m_staticObjects[m_pacman.getPosY()][m_pacman.getPosX()+1]->getType()=='W');
        case 'S':
            if (m_pacman.getPosY() == 9) return true;
            return (m_staticObjects[m_pacman.getPosY()+1][m_pacman.getPosX()]->getType()=='W');
    }
    return false;
}


bool Map::ghostWallCollision() {
    return true;
}

