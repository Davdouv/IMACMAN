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

int Map::getState() const { return m_state;}
int Map::getNbX() const { return m_nbX; }
int Map::getNbY() const { return m_nbY; }
std::string Map::getFileMap() const { return m_fileMap; }
Pacman* Map::getPacman() { return &m_pacman; }
std::vector<Ghost> Map::getGhosts() const { return m_ghosts; }
std::vector<std::vector<StaticObject*>> Map::getStaticObjects() const { return m_staticObjects; }

void Map::setState(int state) { m_state = state;}
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

void Map::initialization() {

    this->setState(Map::State::NORMAL);
    this->m_player.initialization();
    this->load();
}

int Map::load() {

    std::fstream file;
    file.open("../Code/data/"+m_fileMap, std::ios::binary | std::ios::out | std::ios::in);
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
        std::string delimiter = ",";
        std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
        std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        Pacman *p = new Pacman(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.5, 0.005, Object::Orientation::LEFT);
        this->setPacman(*p);
        std::vector<Ghost> tabGhost;
        for (int i = 0; i < 4; i++) {   
            getline(file,tmp);
            std::string delimiter = ",";
            std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
            std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
            Ghost *g = new Ghost(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.5, 0.005, i+1, Object::Orientation::LEFT);
            tabGhost.push_back(*g);
            delete(g);
        }
        this->setGhosts(tabGhost);
        bool once = true;
        while (!file.eof()) {
            getline(file, tmp);
            std::vector<StaticObject*> row;
            if(once)
            {
                 m_nbY = tmp.size();
                 once = false;
            }
            for (int j = 0; j < tmp.size(); j++) {
                StaticObject *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                switch(tmp[j]) {

                    case 'W' : o = new Wall(j, i, 1, 1,  Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(j, i, 0.15, 0.15, Edible::Type::PAC_GOMME, Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(j, i, 0.25, 0.25, Edible::Type::SUPER_PAC_GOMME, Object::Orientation::LEFT);
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
        delete(p);

        m_nbX = i-1;
    }   
    file.close();
    return 1;
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
// For console only
void Map::play() {

    bool play = true;
    std::string line;
    while (play) {
        display();
        std::cout << "Your move : " << std::endl;
        getline(std::cin, line);
        if (line == "Z") {            
            if (!characterWallCollision(&m_pacman, 'Z')) m_pacman.moveUp();
        }
        if (line == "Q") {    
            if (!characterWallCollision(&m_pacman, 'Q')) m_pacman.moveLeft();
        }
        if (line == "S") {   
            if (!characterWallCollision(&m_pacman, 'S')) m_pacman.moveDown();
        }
        if (line == "D") {    
            if (!characterWallCollision(&m_pacman, 'D')) m_pacman.moveRight();
        }
        if (line == "C") play = false;
        pacmanGhostCollision();
        pacmanEdibleCollision();
        ghostMove();
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

// Returns true if can move, false if not
bool Map::moveCharacter(Character* character, Controller::Key action)
{
    switch (action)
	{
		case Controller::Z :
            if (!characterWallCollision(character, 'Z'))
            {
                character->moveUp();
                return true;
            }
			break;
		case Controller::Q :
            if (!characterWallCollision(character, 'Q')) 
            {
                character->moveLeft();
                return true;
            }
			break;
		case Controller::S :
            if (!characterWallCollision(character, 'S'))
            {
                character->moveDown();
                return true;
            }
			break;
		case Controller::D :
            if (!characterWallCollision(character, 'D')) 
            {
                character->moveRight();
                return true;
            }
			break;
		default :
			break;
	}
    return false;
}

void Map::play(Controller* controller) {
    Controller::Key action = controller->getPlayerAction();
    
    if (moveCharacter(&m_pacman, action))
    {
        controller->setPlayerPreviousAction(action);
    }
    // Else keep doing the previous action
    else
    {
	    moveCharacter(&m_pacman, controller->getPlayerPreviousAction());
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
            switch(this->getState()) {
                
                case Map::State::NORMAL : m_player.loseLife();
                    m_pacman.reset();
                    for (int i = 0; i < m_ghosts.size(); i++) {
                        m_ghosts[i].reset();
                    }
                    return;
                case Map::State::SUPER : m_ghosts[i].reset();
                    m_player.gainPoints(1000);
            }
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

bool Map::betweenTwoCells(float characterPos, int cellPos)
{
    float seuil = 0.005;
    if ((characterPos - (float)cellPos) > seuil)
        return true;
    else return false;
}

bool Map::characterWallCollision(Character* character, char direction) {
    int posX = (int)character->getPosX();
    int posY = (int)character->getPosY();

    switch(direction) {
        case 'Z': 
            // First check if we can move inside the cell
            if ((float)posY+character->getSpeed() < character->getPosY())
            {
                // Then make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosX(), posX))
                {
                    // Check if we can go up on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY][posX+1]->getType()=='W'))
                        return true;
                }
                return false;
            }
            // Finally check if we can move to the next cell
            if (posY-1 >= 0)
            {
                if (betweenTwoCells(character->getPosX(), posX))
                    return true;
                return (m_staticObjects[posY-1][posX]->getType()=='W');
            }
                
            else
                return true;
        case 'Q':
            // First check if we can move inside the cell
            if ((float)posX+character->getSpeed() < character->getPosX())
            {
                // Then make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosY(), posY))
                {
                    // Check if we can go left on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY+1][posX]->getType()=='W'))
                    {
                        //std::cout << "Y " << character->getPosY() << " X " << character->getPosX() <<std::endl;
                        return true;
                    }
                }
                return false;
            }
                
            if (posX-1 >= 0)
            {
                //std::cout << (float)posX+character->getSpeed() << " < " << character->getPosX() << std::endl;
                //std::cout << character->getPosY() << std::endl;
                //std::cout << (m_staticObjects[posY][posX-1]->getType()=='W') << std::endl;
                // Then make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosY(), posY))
                    return true;
                return (m_staticObjects[posY][posX-1]->getType()=='W');
            }
            else
                return true;
        case 'D':
            // First check if we can move inside the cell
            if ((float)posX+character->getSpeed() < character->getPosX())
            {
                // Then make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosY(), posY))
                {
                    // Check if we can go left on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY+1][posX]->getType()=='W'))
                        return true;
                }
                return false;
            }
            if (posX+1 <= m_nbY-1)
            {
                // Then make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosY(), posY))
                {
                    // Check if we can go right on both cells
                    if ((m_staticObjects[posY][posX+1]->getType()=='W') || (m_staticObjects[posY+1][posX+1]->getType()=='W'))
                        return true;
                }
                else
                    return (m_staticObjects[posY][posX+1]->getType()=='W');
            }
                
            else
                return true;
        case 'S':
            
            if (posY+1 <= m_nbX-1)
            {
                // Make sure we're not between 2 cells
                if (betweenTwoCells(character->getPosX(), posX))
                {
                    // Check if we can go down on both cells
                    if ((m_staticObjects[posY+1][posX]->getType()=='W') || (m_staticObjects[posY+1][posX+1]->getType()=='W'))
                        return true;
                }
                else
                    return (m_staticObjects[posY+1][posX]->getType()=='W');
            }
            else
            {
                return true;
            }
                
        default:
            break;
    }
    return false;
}

/*
bool Map::ghostWallCollision(int ghostType, char direction) {
    int posX = (int)m_ghosts[ghostType].getPosX();
    int posY = (int)m_ghosts[ghostType].getPosY();
    float seuil = 0.01;
    switch(direction) {
        case 'Z': 
            // First check if we can move inside the cell
            if ((float)posY+m_ghosts[ghostType].getSpeed() < m_ghosts[ghostType].getPosY())
            {
                // Then make sure we're not between 2 cells
                if ((m_ghosts[ghostType].getPosX() - (float)posX) > seuil)
                {
                    // Check if we can go up on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY][posX+1]->getType()=='W'))
                        return true;
                }
                return false;
            }
            // Finally check if we can move to the next cell
            if (posY-1 >= 0)
                return (m_staticObjects[posY-1][posX]->getType()=='W');
            else
                return true;
        case 'Q':
            // First check if we can move inside the cell
            if ((float)posX+m_ghosts[ghostType].getSpeed() < m_ghosts[ghostType].getPosX())
            {
                // Then make sure we're not between 2 cells
                if ((m_ghosts[ghostType].getPosY() - (float)posY) > seuil)
                {
                    std::cout <<(m_ghosts[ghostType].getPosY() - (float)posY)<<std::endl;
                    // Check if we can go left on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY+1][posX]->getType()=='W'))
                        return true;
                }
                return false;
            }
                
            if (posX-1 >= 0)
                return (m_staticObjects[posY][posX-1]->getType()=='W');
            else
                return true;
        case 'D':
            // First check if we can move inside the cell
            if ((float)posX+m_ghosts[ghostType].getSpeed() < m_ghosts[ghostType].getPosX())
            {
                // Then make sure we're not between 2 cells
                if ((m_ghosts[ghostType].getPosY() - (float)posY) > seuil)
                {
                    // Check if we can go left on both cells
                    if ((m_staticObjects[posY][posX]->getType()=='W') || (m_staticObjects[posY+1][posX]->getType()=='W'))
                        return true;
                }
                return false;
            }
            if (posX+1 <= m_nbY-1)
            {
                // Then make sure we're not between 2 cells
                if ((m_ghosts[ghostType].getPosY() - (float)posY) > seuil)
                {
                    // Check if we can go right on both cells
                    if ((m_staticObjects[posY][posX+1]->getType()=='W') || (m_staticObjects[posY+1][posX+1]->getType()=='W'))
                        return true;
                }
                else
                    return (m_staticObjects[posY][posX+1]->getType()=='W');
            }
                
            else
                return true;
        case 'S':
            
            if (posY+1 <= m_nbX-1)
            {
                // Make sure we're not between 2 cells
                if ((m_ghosts[ghostType].getPosX() - (float)posX) > seuil)
                {
                    // Check if we can go down on both cells
                    if ((m_staticObjects[posY+1][posX]->getType()=='W') || (m_staticObjects[posY+1][posX+1]->getType()=='W'))
                        return true;
                }
                else
                    return (m_staticObjects[posY+1][posX]->getType()=='W');
            }
            else
            {
                return true;
            }
                
        default:
            break;
    }
    return false;
}*/

void Map::pacmanEdibleCollision() {

    if (m_staticObjects[m_pacman.getPosY()][m_pacman.getPosX()]->getType()=='E') {
        Edible *e;
        e =  (Edible*) m_staticObjects[m_pacman.getPosY()][m_pacman.getPosX()];
        m_player.gainPoints(e->gain());
        if (e->getTypeEdible() == Edible::Type::SUPER_PAC_GOMME) this->setState(Map::State::SUPER); 
        m_staticObjects[m_pacman.getPosY()][m_pacman.getPosX()]->setType('V');
    }   
}

// Shadow will follow Pacman all along, so he will find the shortest way to go to Pacman
void Map::shadowAI() {

    shortestWay(Ghost::Type::SHADOW, m_pacman.getPosX(), m_pacman.getPosY());
}

// Speedy aims the direction Pacman is going so he find the shortest way to this direction
void Map::speedyAI() {

    switch(m_pacman.getOrientation()) {

        case Object::Orientation::LEFT:
            shortestWay(Ghost::Type::SPEEDY, 0, m_pacman.getPosY());
            break;

        case Object::Orientation::RIGHT:
            shortestWay(Ghost::Type::SPEEDY, m_nbX, m_pacman.getPosY());
            break;

        case Object::Orientation::UP:
            shortestWay(Ghost::Type::SPEEDY, m_pacman.getPosX(), 0);
            break;

        case Object::Orientation::DOWN:
            shortestWay(Ghost::Type::SPEEDY, m_pacman.getPosX(), m_nbY);
            break;
        default:break;
    }
    
}

// When Pacman gets really close to Bashful, he goes to Pacman's opposite direction
// else it keeps moving the way it is
// moves like Speedy until he gets close to pacman

void Map::bashfulAI() {

    if ((abs(m_pacman.getPosX() - m_ghosts[Ghost::Type::BASHFUL].getPosX()) <= 2)  && (abs(m_pacman.getPosY() - m_ghosts[Ghost::Type::BASHFUL].getPosY()) <= 10)) {

        switch(m_pacman.getOrientation()) {

            case Object::Orientation::LEFT:
                shortestWay(Ghost::Type::BASHFUL, m_nbX, m_pacman.getPosY());
                break;

            case Object::Orientation::RIGHT:
                shortestWay(Ghost::Type::BASHFUL, 0, m_pacman.getPosY());
                break;

            case Object::Orientation::UP:
                shortestWay(Ghost::Type::BASHFUL, m_pacman.getPosX(), m_nbY);
                break;

            case Object::Orientation::DOWN:
                shortestWay(Ghost::Type::BASHFUL, m_pacman.getPosX(), 0);
                break;
            default:break;
        }
    }

    else {
    
        switch(m_pacman.getOrientation()) {

            case Object::Orientation::LEFT:
                shortestWay(Ghost::Type::BASHFUL, 0, m_pacman.getPosY());
                break;

            case Object::Orientation::RIGHT:
                shortestWay(Ghost::Type::BASHFUL, m_nbX, m_pacman.getPosY());
                break;

            case Object::Orientation::UP:
                shortestWay(Ghost::Type::BASHFUL, m_pacman.getPosX(), 0);
                break;

            case Object::Orientation::DOWN:
                shortestWay(Ghost::Type::BASHFUL, m_pacman.getPosX(), m_nbY);
                break;
            default:break;
        }
    }
}

// goes around randomly
void Map::pokeyAI() {
    
    int rx = (rand()/RAND_MAX) * m_nbX;
    int ry = (rand()/RAND_MAX) * m_nbY;
    shortestWay(Ghost::Type::POKEY, rx, rx);
}

// Shortest way for a ghost to get to the position (x, y)
// returns 1 if the goal is achieved so we set another one
// returns 0 if the goal isn't achieved yet
int Map::shortestWay(int ghostType, float x, float y) {

    float gx = m_ghosts[ghostType].getPosX();
    float gy = m_ghosts[ghostType].getPosY();
    if ((gx != x) && (gy != y)) {

        // if goal is at the right top
        if ((gx - x < 0) && (gy - y < 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
        }
        // if goal is at the right bottom
        else if ((gx - x < 0) && (gy - y > 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
            if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
        }
        // if goal is at the left top
        else if ((gx - x > 0) && (gy - y < 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
        }
        // if goal is at the left bottom
        else if ((gx - x > 0) && (gy - y > 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
        }
        // if goal is at the right
        else if ((gx - x < 0) && (gy - y == 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
        }
        // if goal is at the left
        else if ((gx - x > 0) && (gy - y == 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
        }
        // if goal is at the top
        else if ((gx - x == 0) && (gy - y < 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
        }
        // if goal is at the bottom
        else if ((gx - x == 0) && (gy - y > 0)) {
            if (!characterWallCollision(&m_ghosts[ghostType], 'S')) m_ghosts[ghostType].moveDown();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Z')) m_ghosts[ghostType].moveUp();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'Q')) m_ghosts[ghostType].moveLeft();
            else if (!characterWallCollision(&m_ghosts[ghostType], 'D')) m_ghosts[ghostType].moveRight();
        }
        return 0;
    }
    return 1;
}

void Map::ghostMove() {

    shadowAI();
    speedyAI();
    bashfulAI();
    pokeyAI();
}