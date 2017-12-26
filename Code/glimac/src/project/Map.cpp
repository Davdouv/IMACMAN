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
        while (!file.eof()) {
            getline(file, tmp);
            std::vector<StaticObject*> row;
            for (int j = 0; j < tmp.size(); j++) {
                StaticObject *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                switch(tmp[j]) {

                    case 'W' : o = new Wall(i, j, 10, 10,  Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(i, j, 10, 10, 1,  Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(i, j, 10, 10, 2,  Object::Orientation::LEFT);
                        break;
                    case 'B' : o = new Edible(i, j, 10, 10, 3, Object::Orientation::LEFT);
                        break;
                    case 'V' : o = NULL;
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
/*
void Map::play() {

    bool play = true;
    std::string line;
    while (play) {
        display();
        std::cout << "Your move : " << std::endl;
        getline(std::cin, line);
        if (line == "Z") {            
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()+1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveUp();
            } 
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');

        }
        if (line == "Q") {
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()-1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveLeft();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
        }
        if (line == "S") {
            if (m_cells[m_pacman.getPosX()][m_pacman.getPosY()-1].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveDown();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
        }
        if (line == "D") {
            if (m_cells[m_pacman.getPosX()+1][m_pacman.getPosY()].getAccess()) {
                m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('V');
                m_pacman.moveRight();
            }
            m_cells[m_pacman.getPosX()][m_pacman.getPosY()].setStaticElement('P');
        }
        if (line == "C") play = false;
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

    m_pacman.display();
    for (int i = 0; i < 10; i++) {
        std::vector<StaticObject*> tmp = m_staticObjects.at(i);
        for (int  j = 0; j < 10; j++) {
            if (tmp.at(j) != NULL)tmp.at(j)->display();
            else std::cout << "Vide" << std::endl;
        }
        std::cout << std::endl;
    }
}

void Map::pacmanGhostCollision() {

    for (int i = 0; i < m_ghosts.size(); i++) {
        if (m_pacman.collision(m_ghosts[i])) m_ghosts[i].reset();
    }
}

bool Map::ghostCollision() {

    for (int i = 0; i < m_ghosts.size(); i++) {
        for (int j = 0; j < m_ghosts.size() && j!=i; j++) {
               if (m_ghosts[i].collision(m_ghosts[j])) return true;
        }
    }
    return false;
}

bool Map::wallCollision(Cell c) {
    return (c.getAccess());
}


