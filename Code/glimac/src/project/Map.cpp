#include "../include/project/Object.hpp"
#include "../include/project/Map.hpp"
#include "../include/project/Wall.hpp"
#include "../include/project/Edible.hpp"
#include "../include/project/Controller.hpp"
#include <string>
#include <iostream>

Map::Map() { }
std::string Map::getFileMap() const { return m_fileMap; }

void Map::setFileMap(std::string fileName) { m_fileMap = fileName; }

int Map::load() {

    std::fstream file;
    file.open(m_fileMap, std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << " error loading the file " << std::endl;
        return 0;
    }
    else {
        int i = 0;
        std::string tmp;
        file.seekg(0);
        while (!file.eof()) {
            getline(file, tmp);
            for (int j = 0; j < tmp.size(); j++) {
                Object *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                m_cells[i][j].setAccess(1);
                m_cells[i][j].setStaticElement(tmp[j]);
                switch(tmp[j]) {

                    case 'P' : o = new Pacman(i, j, 10, 10, 1, Object::Orientation::LEFT);
                        m_pacman.setPosX(i);
                        m_pacman.setPosY(j);
                        m_pacman.setWidth(10);
                        m_pacman.setHeight(10);
                        m_pacman.setSpeed(1); 
                        break;
                    case 'W' : o = new Wall(i, j, 10, 10,  Object::Orientation::LEFT);
                        m_cells[i][j].setAccess(0);
                        break;
                    case 'A' : o = new Ghost(i, j, 10, 10, 1, 1,  Object::Orientation::LEFT);
                        break;
                    case 'B' : o = new Ghost(i, j, 10, 10, 1, 2,  Object::Orientation::LEFT);
                        break;
                    case 'C' : o = new Ghost(i, j, 10, 10, 1, 3,  Object::Orientation::LEFT);
                        break;
                    case 'D' : o = new Ghost(i, j, 10, 10, 1, 4,  Object::Orientation::LEFT);
                        break;
                    case '1' : o = new Edible(i, j, 10, 10, 1,  Object::Orientation::LEFT);
                        m_cells[i][j].addObject(o);
                        o = new Ghost(i, j, 10, 10, 1, 1,  Object::Orientation::LEFT);
                        break;
                    case '2' : o = new Edible(i, j, 10, 10, 2,  Object::Orientation::LEFT);
                        m_cells[i][j].addObject(o);
                        o = new Ghost(i, j, 10, 10, 1, 1, Object::Orientation::LEFT);
                        break;
                    case '3' : o = new Edible(i, j, 10, 10, 3, Object::Orientation::LEFT);
                        m_cells[i][j].addObject(o);
                        o = new Ghost(i, j, 10, 10, 1, 1, Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(i, j, 10, 10, 1, Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(i, j, 10, 10, 2, Object::Orientation::LEFT);
                        break;
                    case 'O' : o = new Edible(i, j, 10, 10, 3, Object::Orientation::LEFT);
                        break;
                    case 'V' : m_cells[i][j].setEmpty(true);
                        break;
                    default : break;
                }
                if (!m_cells[i][j].getEmpty()) m_cells[i][j].addObject(o);
            }
            i++;
        }
    }   
    file.close();
    return 1;
}

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

    for (int i = 0; i < m_nbX; i++) {
        for (int  j = 0; j < m_nbY; j++) m_cells[i][j].display();
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


