#include "project/GameManager.hpp"
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>
#include <SDL/SDL_mixer.h>

using namespace glimac;

static int once = 0;
GameManager::GameManager(Map* map)
{
    m_map = map;
    m_state = NORMAL;
    m_player.initialization();
    m_pause = false;
    m_lost = false;
}

GameManager::PacmanState GameManager::getState() const { return m_state;}
uint32_t GameManager::getStartTime() const { return m_startTime; }
uint32_t GameManager::getSuperTimer() const { return m_superTimer; }
uint32_t GameManager::getFruitTimer() const { return m_fruitTimer; }
bool GameManager::isPause() { return m_pause; }
int GameManager::getEatenGhosts() const { return m_eatenGhosts; }
Player* GameManager::getPlayer() { return &m_player; }
Map* GameManager::getMap() { return m_map; }
void GameManager::setState(PacmanState state) { m_state = state;}
void GameManager::setStartTime(uint32_t t) { m_startTime = t;}
void GameManager::setSuperTimer(uint32_t t) { m_superTimer = t;}
void GameManager::setFruitTimer(uint32_t t) { m_fruitTimer = t; }
void GameManager::switchPause() { m_pause=!isPause();}
void GameManager::setEatenGhosts(int eatenGhosts) { m_eatenGhosts = eatenGhosts;}

// FILE MANAGER

int GameManager::load(bool newGame) {

    std::fstream file;
    if (!newGame)
        file.open("../Code/data/save_"+m_map->getFileMap(), std::ios::binary | std::ios::out | std::ios::in);
    else file.open("../Code/data/"+m_map->getFileMap(), std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << " error loading the file " << std::endl;
        return 0;
    }
    else {
        std::vector<std::vector<StaticObject*>> vec;
        int i = 0;
        std::string tmp;
        file.seekg(0);

        // if saved game, load lives and points
        if (!newGame) {
            getline(file,tmp);
            m_player.setLife(atoi(tmp.c_str()));
            getline(file,tmp);
            m_player.setPoints(atoi(tmp.c_str()));
        }
        getline(file,tmp);
        std::string delimiter = ",";
        std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
        std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        Pacman *p = new Pacman(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.5, 0.004, Object::Orientation::LEFT);


        // if saved game get pacman initial positions
        if (!newGame) {
        getline(file,tmp);
            std::string delimiter = ",";
            std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
            std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
            p->setInitX(atoi(pos_x.c_str()));
            p->setInitY(atoi(pos_y.c_str()));
        }

        m_map->setPacman(*p);

        std::vector<Ghost> tabGhost;
        for (int i = 0; i < 4; i++) {
            getline(file,tmp);
            std::string delimiter = ",";
            std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
            std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
            Ghost *g = new Ghost(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.75, 0.004, i, Object::Orientation::LEFT, 0);
            if (!newGame) {
                getline(file,tmp);
                std::string delimiter = ",";
                std::string init_x = tmp.substr(1, tmp.find(delimiter)-1);
                std::string init_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
                g->setInitX(atoi(init_x.c_str()));
                g->setInitY(atoi(init_y.c_str()));
            }
            tabGhost.push_back(*g);
            delete(g);
        }
        m_map->setGhosts(tabGhost);
        bool once = true;
        while (!file.eof()) {
            getline(file, tmp);
            std::vector<StaticObject*> row;
            if(once)
            {
                m_map->setNbY(tmp.size());
                once = false;
            }
            for (int j = 0; j < tmp.size(); j++) {
                StaticObject *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                switch(tmp[j]) {

                    case 'W' : o = new Wall(j, i, 1, 1,  Object::Orientation::LEFT);
                        break;
                    case 'G' : o = new Edible(j, i, 0.20, 0.20, Edible::Type::PAC_GOMME, true, Edible::Fruit::NONE,  Object::Orientation::LEFT);
                        break;
                    case 'S' : o = new Edible(j, i, 0.30, 0.30, Edible::Type::SUPER_PAC_GOMME, true, Edible::Fruit::NONE, Object::Orientation::LEFT);
                        break;
                    case 'B' : o = new Edible(j, i, 0.3, 0.3, Edible::Type::FRUIT, false, Edible::Fruit::APPLE, Object::Orientation::LEFT);
                        break;
                    case 'D' : o = new Door(j, i, 1, 1, Object::Orientation::LEFT);
                        break;
                    case 'V' : o = new StaticObject('V', j, i, 1, 1, Object::Orientation::LEFT);
                        break;
                    case 'X' : o = new StaticObject('X', j, i, 1, 1, Object::Orientation::LEFT);
                    default : break;
                }
                row.push_back(o);
            }
            vec.push_back(row);
            i++;
        }
        m_map->setStaticObjects(vec);
        delete(p);

        m_map->setNbX(i-1);
    }
    file.close();
    return 1;
}


int GameManager::save() {

    std::fstream file;
    file.open("../Code/data/save_"+m_map->getFileMap(), std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::ofstream outfile("../Code/data/save_"+m_map->getFileMap());
        outfile.close();
        save();
    }
    else {
        file.seekg(0);
        // save player lives
        file << m_player.getLife() << std::endl;
        // save player points
        file << m_player.getPoints() << std::endl;

        // current pacman position
        file << "P" << m_map->getPacman()->getPosX() << "," << m_map->getPacman()->getPosY() << std::endl;
        // initial pacman position
        file << "P" << m_map->getPacman()->getInitX() << "," << m_map->getPacman()->getInitY() << std::endl;

        // ghosts positions
        file << "G" << m_map->getGhosts()[Ghost::Type::SHADOW]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::SHADOW]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SHADOW]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::SHADOW]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SPEEDY]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::SPEEDY]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::POKEY]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::POKEY]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::POKEY]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::POKEY]->getInitY() << std::endl;

        for (int i = 0; i < m_map->getNbX(); i++) {
            for (int j = 0; j < m_map->getNbY(); j++) {
                if (m_map->getStaticObjects()[i][j]->getType() == 'E') {
                    Edible *e = (Edible*) m_map->getStaticObjects()[i][j];
                    switch (e->getTypeEdible()) {

                        case Edible::Type::PAC_GOMME: file << 'G';
                            break;
                        case Edible::Type::SUPER_PAC_GOMME: file << 'S';
                            break;
                        case Edible::Type::FRUIT: file << e->getFruit();
                            break;
                    }
                }
                else file << m_map->getStaticObjects()[i][j]->getType();
            }
            file << std::endl;
        }
    }
    file.close();
    return 1;
}

// returns true if no edible on the map
bool GameManager::won() {

    return (m_map->getSuperPacGommes().empty() && m_map->getPacGommes().empty());
}

// returns true if the players lost all his lives
bool GameManager::lost() {

    return (!m_player.getLife());
}

bool GameManager::ready() {

    return (SDL_GetTicks() - getStartTime() > 1000);
}

void GameManager::pause(Controller* controller) {

    if (controller->getInterfaceAction() == Controller::ESCAPE)
    {
                switchPause();
                controller->setInterfaceAction(Controller::NONE);
                if (m_pause) Mix_PauseMusic();
                else Mix_ResumeMusic();
    }

}

void GameManager::start(AudioManager* audioManager) {

    setTimers();
    play(audioManager);
}

// For console only
void GameManager::play(AudioManager* audioManager) {

    Controller * c;
    m_map->getPacman()->setSpeed(1);
    for (int i = 0; i < m_map->getGhosts().size(); i++) m_map->getGhosts()[i]->setSpeed(1);
    std::string line;
    this->setStartTime(SDL_GetTicks());
    setTimers();
    while (!(this->won())) {
        if (ready()) {
            stateManager();
            activateFruit();
            m_map->display();
            std::cout << "Your move : " << std::endl;
            getline(std::cin, line);
            if (line == "Z") {
                if (!characterWallCollision(m_map->getPacman(), 'Z')) m_map->getPacman()->moveUp();
            }
            if (line == "Q") {
                if (!characterWallCollision(m_map->getPacman(), 'Q')) m_map->getPacman()->moveLeft();
            }
            if (line == "S") {
                if (!characterWallCollision(m_map->getPacman(), 'S')) m_map->getPacman()->moveDown();
            }
            if (line == "D") {
                if (!characterWallCollision(m_map->getPacman(), 'D')) m_map->getPacman()->moveRight();
            }
            if (line == "C") {
                save();
                return;
            }
            pacmanGhostCollision(audioManager);
            pacmanEdibleCollision(audioManager);
            ghostMove();
            std::cout << "Points : " << m_player.getPoints() << std::endl;
            std::cout << "Lives : " << m_player.getLife() << std::endl;
        }
    }
}

// Returns true if can move, false if not (or if he takes door)
bool GameManager::moveCharacter(Character* character, Controller::Key action)
{
    bool pacman = false;
    if (character->getType() == 'P') pacman = true;
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
            if (characterLeftDoorCollision(character))
            {
                Door* d = (Door*) (m_map->getStaticObjects())[character->getPosY()][character->getPosX()];
                character->setPosX(d->getDestX());
                character->setPosY(d->getDestY());
                character->moveLeft();
                return false;
            }
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
            if (characterRightDoorCollision(character))
            {
                Door* d = (Door*) (m_map->getStaticObjects())[character->getPosY()][character->getPosX()+1];
                character->setPosX(d->getDestX());
                character->setPosY(d->getDestY());
                character->moveRight();
                return false;
            }
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

void GameManager::pacmanMove(Controller* controller)
{
    Controller::Key action = controller->getPlayerAction();

    if (moveCharacter(m_map->getPacman(), action))
    {
        controller->setPlayerPreviousAction(action);
    }
    // Else keep doing the previous action
    else
    {
	    moveCharacter(m_map->getPacman(), controller->getPlayerPreviousAction());
    }
}


// --- PLAY FUNCTION | Call all other functions --- //
void GameManager::play(Controller* controller, AudioManager* audioManager) {

    if (!(lost()) && ready() && !isPause())
    {
        stateManager();
        activateFruit();
        pacmanMove(controller);
        ghostMove();
        pacmanEdibleCollision(audioManager);
        if (pacmanGhostCollision(audioManager))
        {
            controller->setPlayerAction(Controller::Key::Q);
        }
        if(won())
        {
            newLevel(controller);
        }
        if(lost())
        {
            m_lost = true;
            std::cout << "Player Score : " << m_player.getPoints() << std::endl;
        }
    }
    pause(controller);
}

void GameManager::newLevel(Controller* controller)
{
    controller->setPlayerPreviousAction(Controller::Key::Q);
    setState(NORMAL);
    Player save_player = m_player;
    m_map->initialization();
    load(true);
    m_player = save_player;
    setTimers();
}

bool GameManager::pacmanGhostCollision(AudioManager* audioManager) {

    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        if (m_map->getPacman()->collision(m_map->getGhosts()[i])) {
            if(!(m_map->getGhosts()[i]->getSuper())) {
                m_player.loseLife();
                if (m_player.getLife() != 0)
                {
                    std::cout << m_player.getLife() << std::endl;
                    m_map->getPacman()->reset();
                    setStartTime(SDL_GetTicks());
                    audioManager->playSound(2,1);
                }
                else // We're dead
                {
                    audioManager->playSound(7,1);
                    Mix_PauseMusic();
                }
                for (int i = 0; i < m_map->getGhosts().size(); i++) {
                    m_map->getGhosts()[i]->reset();
                }
                std::cout << "Life lost. Life : " << m_player.getLife() << std::endl;
                return true;
            }
            else {
                m_map->getGhosts()[i]->reset();
                eatGhost();
                audioManager->playSound(3,1);
            }
        }
    }
    return false;
}

bool GameManager::ghostCollision() {

    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        for (int j = 0; j < m_map->getGhosts().size() && j!=i; j++) {
               if (m_map->getGhosts()[i]->collision(m_map->getGhosts()[j])) return true;
        }
    }
    return false;
}

bool GameManager::wallCollisionUP(float fposY, int iposY, int iposX, float speed, Character* character)
{
    // Check if we're still going to be inside the cell
    if((fposY - speed) > iposY)
    {
        // Put the character on the left edge of the cell
        character->setPosX((float)iposX);
        // Ok we can move inside the cell
        return false;
    }
    // We're going to be on the upper cell
    else
    {
        // Check if we can go on the upper cell
        if(iposY >= 1)
        {
            // Check if upper cell is a wall
            if (m_map->getStaticObjects()[iposY-1][iposX]->getType()=='W')
            {
                // It's a wall, return true -> it's a collision, put the character on the top of the cell
                character->setPosY((float)iposY);
                return true;
            }
            else if (character->isPacman() && m_map->getStaticObjects()[iposY-1][iposX]->getType()=='X')
            {
                // It's the spawn gate, return true -> it's a collision, put the character on the top of the cell
                character->setPosY((float)iposY);
                return true;
            }
            else
            {
                // Put the character on the left edge of the cell
                character->setPosX((float)iposX);
                // It's ok we can move to the other cell
                return false;
            }
        }
        else
        {
            // We are at the edge of the game
            character->setPosY((float)iposY);
            return true;
        }
    }
}

bool GameManager::wallCollisionLEFT(float fposX, int iposY, int iposX, float speed, Character* character)
{
    if((fposX - speed) > iposX)
    {
        character->setPosY((float)iposY);
        return false;
    }
    else
    {
        if(iposX >= 1)
        {
            if (m_map->getStaticObjects()[iposY][iposX-1]->getType()=='W')
            {
                character->setPosX((float)iposX);
                return true;
            }
            else if (character->isPacman() && m_map->getStaticObjects()[iposY][iposX-1]->getType()=='X')
            {
                character->setPosX((float)iposX);
                return true;
            }
            else
            {
                character->setPosY((float)iposY);
                return false;
            }
        }
        else
        {
            character->setPosX((float)iposX);
            return true;
        }
    }
}

bool GameManager::wallCollisionDOWN(float fposY, int iposY, int iposX, float speed, Character* character)
{
    // Check if we can go on the bottom cell
    if(iposY+1 <= m_map->getNbX()-1)
    {
        // Check if bottom cell is a wall
        if (m_map->getStaticObjects()[iposY+1][iposX]->getType()=='W')
        {
            character->setPosY((float)iposY);
            return true;
        }
        else if (character->isPacman() && m_map->getStaticObjects()[iposY+1][iposX]->getType()=='X')
        {
            character->setPosY((float)iposY);
            return true;
        }
        else
        {
            character->setPosX((float)iposX);
            return false;
        }
    }
    else
    {
        character->setPosY((float)iposY);
        return true;
    }
}

bool GameManager::wallCollisionRIGHT(float fposX, int iposY, int iposX, float speed, Character* character)
{
    if(iposX+1 <= m_map->getNbY()-1)
    {
        if (m_map->getStaticObjects()[iposY][iposX+1]->getType()=='W')
        {
            character->setPosX((float)iposX);
            return true;
        }
        else if (character->isPacman() && m_map->getStaticObjects()[iposY][iposX+1]->getType()=='X')
        {
            character->setPosX((float)iposX);
            return true;
        }
        else
        {
            character->setPosY((float)iposY);
            return false;
        }
    }
    else
    {
        character->setPosX((float)iposX);
        return true;
    }
}

bool GameManager::characterWallCollision(Character* character, char direction) {
    float fposX = character->getPosX();
    float fposY = character->getPosY();
    int iposX = (int)fposX;   // Matrix index X
    int iposY = (int)fposY;   // Matrix index Y
    float speed = character->getSpeed();
    float seuil = 0.021;

    switch(direction) {
        case 'Z':
            // Check if we are close to the left edge
            if(fposX - iposX <= seuil)
            {
                return wallCollisionUP(fposY, iposY, iposX, speed, character);
            }
            /* Finally we want the player to go through each cells */
            // Check if we are close to the right edge
            // else if (iposX+1 - fposX <= seuil)
            // {
            //     // Consider we're on the right edge
            //     return wallCollisionUP(fposY, iposY, iposX+1, speed, character);
            // }
            // We are between two cells
            else
            {
                // We can't go up
                return true;
            }
            break;
        case 'Q':
            if(fposY - iposY <= seuil)
            {
                return wallCollisionLEFT(fposX, iposY, iposX, speed, character);
            }
            /* Finally we want the player to go through each cells */
            // else if (iposY+1 - fposY <= seuil)
            // {
            //     return wallCollisionLEFT(fposX, iposY+1, iposX, speed, character);
            // }
            else
            {
                return true;
            }
            break;
        case 'S':
            if(fposX - iposX <= seuil)
            {
                return wallCollisionDOWN(fposY, iposY, iposX, speed, character);
            }
            else if (iposX+1 - fposX <= seuil)
            {
                return wallCollisionDOWN(fposX, iposY, iposX+1, speed, character);
            }
            else
            {
                return true;
            }
            break;
        case 'D':
            if(fposY - iposY <= seuil)
            {
                return wallCollisionRIGHT(fposX, iposY, iposX, speed, character);
            }
            else if (iposY+1 - fposY <= seuil)
            {
                return wallCollisionRIGHT(fposX, iposY+1, iposX, speed, character);
            }
            else
            {
                return true;
            }
            break;
        default:
            return true;
            break;
    }

}

bool GameManager::characterLeftDoorCollision(Character* character)
{
    float fposX = character->getPosX();
    int iposX = (int)fposX;
    int iposY = character->getPosY();

    if (m_map->getStaticObjects()[iposY][iposX]->getType()=='D')
    {
        if (fposX - iposX < 0.1)
        {
            return true;
        }
    }
    return false;
}

bool GameManager::characterRightDoorCollision(Character* character)
{
    float fposX = character->getPosX();
    int iposX = (int)fposX;
    int iposY = character->getPosY();

    if (m_map->getStaticObjects()[iposY][iposX+1]->getType()=='D')
    {
        if (iposX+1 - fposX < 0.1)
            return true;
    }
    return false;
}

bool GameManager::pacmanEdibleCollision(AudioManager* audioManager) {
    // If we're going left, we want Pacman to be half inside the cell
    if ((m_map->getPacman()->getPosX() - (int)m_map->getPacman()->getPosX()) > m_map->getPacman()->getWidth())
        return false;
    // If we're going up, we want Pacman to be half inside the cell
    if ((m_map->getPacman()->getPosY() - (int)m_map->getPacman()->getPosY()) > m_map->getPacman()->getHeight())
        return false;
    if (m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->getType()=='E'){
        //std::cout << "we're here ! " << std::endl;
        Edible *e;
        e =  (Edible*) m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()];
        switch (e->getTypeEdible()) {

            case Edible::Type::FRUIT :
                audioManager->playSound(6,1);
                if (e->getAvailability() && e->getFruit()!=Edible::Fruit::NONE) {
                    m_player.gainPoints(e->gain());
                    e->upgradeFruit();
                    setFruitTimer(SDL_GetTicks());
                    if (e->getFruit() == Edible::Fruit::NONE) m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->setType('V');
                    else m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()] = e;
                }
                return true;
                break;

            case Edible::Type::PAC_GOMME :
                audioManager->playSound(1,1);
                m_player.gainPoints(e->gain());
                m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->setType('V');
                return true;
                break;

            case Edible::Type::SUPER_PAC_GOMME :
                Mix_PauseMusic();
                audioManager->playSound(4,1);
                audioManager->playSound(5,1);
                m_player.gainPoints(e->gain());
                switchSuperState();
                m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->setType('V');
                return true;
                break;
            default:return false;
        }
        // std::cout << "Points : " << m_player.getPoints() << std::endl;
        // std::cout << "Life : " << m_player.getLife() << std::endl;
    }
    return false;
}

void GameManager::switchSuperState() {
    this->setState(GameManager::PacmanState::SUPER);
    this->setSuperTimer(SDL_GetTicks());
    this->setEatenGhosts(0);
    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        m_map->getGhosts()[i]->setSuper(true);
        //m_map->getGhosts()[i]->slowDown();
    }
}

void GameManager::stateManager() {

    if (this->getState() == GameManager::PacmanState::SUPER) {
        int timer = 7000; // 1 second * 1000
        if (SDL_GetTicks() - this->getSuperTimer() > timer) {
            Mix_ResumeMusic();
            this->setState(GameManager::PacmanState::NORMAL);
            this->setEatenGhosts(0);
            for (int i = 0; i < m_map->getGhosts().size(); i++) {
                m_map->getGhosts()[i]->setSuper(false);
            }
        }
    }
}

void GameManager::eatGhost() {

    this->setEatenGhosts(this->getEatenGhosts()+1);
    switch (this->getEatenGhosts()) {
        case 1: m_player.gainPoints(200);
            break;
        case 2: m_player.gainPoints(400);
            break;
        case 3: m_player.gainPoints(800);
            break;
        case 4: m_player.gainPoints(1600);
            break;
        default:return;
    }
}
// Shadow will follow Pacman all along, so he will find the shortest way to go to Pacman
void GameManager::shadowAI() {

    if (m_map->getGhosts()[Ghost::Type::SHADOW]->getDeath()) {

        shortestWay(Ghost::Type::SHADOW, m_map->getGhosts()[Ghost::Type::SHADOW]->getInitY(), m_map->getGhosts()[Ghost::Type::SHADOW]->getInitX());
    }
    else shortestWay(Ghost::Type::SHADOW, m_map->getPacman()->getPosX(), m_map->getPacman()->getPosY());
}

// Speedy aims the direction Pacman is going so he find the shortest way to this direction
void GameManager::speedyAI() {

    if (m_map->getGhosts()[Ghost::Type::SPEEDY]->getDeath()) {

        shortestWay(Ghost::Type::SPEEDY, m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitY(), m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitX());
    }
    else {
        switch(m_map->getPacman()->getOrientation()) {

            case Object::Orientation::LEFT:
                shortestWay(Ghost::Type::SPEEDY, 0, m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::RIGHT:
                shortestWay(Ghost::Type::SPEEDY, m_map->getNbX(), m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::UP:
                shortestWay(Ghost::Type::SPEEDY, m_map->getPacman()->getPosX(), 0);
                break;

            case Object::Orientation::DOWN:
                shortestWay(Ghost::Type::SPEEDY, m_map->getPacman()->getPosX(), m_map->getNbY());
                break;
            default:break;
        }
    }

}

// When Pacman gets really close to Bashful, he goes to Pacman's opposite direction
// else it keeps moving the way it is
// moves like Speedy until he gets close to pacman

void GameManager::bashfulAI() {

    if (m_map->getGhosts()[Ghost::Type::BASHFUL]->getDeath()) {

        shortestWay(Ghost::Type::BASHFUL, m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitY(), m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitX());
    }
    else if ((std::abs(m_map->getPacman()->getPosX() - m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosX()) <= 2)  && (std::abs(m_map->getPacman()->getPosY() - m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosY()) <= 10)) {

        switch(m_map->getPacman()->getOrientation()) {

            case Object::Orientation::LEFT:
                shortestWay(Ghost::Type::BASHFUL, m_map->getNbX(), m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::RIGHT:
                shortestWay(Ghost::Type::BASHFUL, 0, m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::UP:
                shortestWay(Ghost::Type::BASHFUL, m_map->getPacman()->getPosX(), m_map->getNbY());
                break;

            case Object::Orientation::DOWN:
                shortestWay(Ghost::Type::BASHFUL, m_map->getPacman()->getPosX(), 0);
                break;
            default:break;
        }
    }

    else {

        switch(m_map->getPacman()->getOrientation()) {

            case Object::Orientation::LEFT:
                shortestWay(Ghost::Type::BASHFUL, 0, m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::RIGHT:
                shortestWay(Ghost::Type::BASHFUL, m_map->getNbX(), m_map->getPacman()->getPosY());
                break;

            case Object::Orientation::UP:
                shortestWay(Ghost::Type::BASHFUL, m_map->getPacman()->getPosX(), 0);
                break;

            case Object::Orientation::DOWN:
                shortestWay(Ghost::Type::BASHFUL, m_map->getPacman()->getPosX(), m_map->getNbY());
                break;
            default:break;
        }
    }
}

// goes around randomly
void GameManager::pokeyAI() {

    if (m_map->getGhosts()[Ghost::Type::POKEY]->getDeath()) {

        shortestWay(Ghost::Type::POKEY, m_map->getGhosts()[Ghost::Type::POKEY]->getInitY(), m_map->getGhosts()[Ghost::Type::POKEY]->getInitX());
    }
    else {
        int rx = (rand()/RAND_MAX) * m_map->getNbX();
        int ry = (rand()/RAND_MAX) * m_map->getNbY();
        shortestWay(Ghost::Type::POKEY, rx, ry);
    }
}

// Shortest way for a ghost to get to the position (x, y)
// returns 1 if the goal is achieved so we set another one
// returns 0 if the goal isn't achieved yet
int GameManager::shortestWay(int ghostType, float x, float y) {

    float gx = m_map->getGhosts()[ghostType]->getPosX();
    float gy = m_map->getGhosts()[ghostType]->getPosY();
    if ((gx != x) && (gy != y)) {

        // if goal is at the right top
        if ((gx - x < 0) && (gy - y < 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
        }
        // if goal is at the right bottom
        else if ((gx - x < 0) && (gy - y > 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
        }
        // if goal is at the left top
        else if ((gx - x > 0) && (gy - y < 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
        }
        // if goal is at the left bottom
        else if ((gx - x > 0) && (gy - y > 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
        }
        // if goal is at the right
        else if ((gx - x < 0) && (gy - y == 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
        }
        // if goal is at the left
        else if ((gx - x > 0) && (gy - y == 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
        }
        // if goal is at the top
        else if ((gx - x == 0) && (gy - y < 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
        }
        // if goal is at the bottom
        else if ((gx - x == 0) && (gy - y > 0)) {
            if (!characterWallCollision(m_map->getGhosts()[ghostType], 'S')) m_map->getGhosts()[ghostType]->moveDown();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Z')) m_map->getGhosts()[ghostType]->moveUp();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'Q')) m_map->getGhosts()[ghostType]->moveLeft();
            else if (!characterWallCollision(m_map->getGhosts()[ghostType], 'D')) m_map->getGhosts()[ghostType]->moveRight();
        }
        return 0;
    }
    return 1;
}


/* From a starting position dx, dy
We calculate the shortest way to get to a goal from:
- dx+1, dy
- dx-1, dy
- dx, dy+1
- dx, dy-1

*/

int GameManager::countShortestWay(int dx, int dy, int ax, int ay, std::vector<std::vector<int>> passage) {

    std::cout << "( "<<dx<<", "<<dy<<" ) ( "<<ax<<", "<<ay<<" )( "<<m_map->getNbX()<<", "<<m_map->getNbY()<<" )" << std::endl;
    if ((dx == ax) && (dy == ay)) {
        std::cout << "***********" << std::endl << "On est arrivés." << std::endl;
        return 0;
    }
    if (dx < 0) return 1000;

    if (dy < 0) return 1000;

    if (dx >= m_map->getNbX()) return 1000;

    if (dy >= m_map->getNbY()) return 1000;

    if (m_map->getStaticObjects()[dy][dx]->getType()=='W') {
        std::cout << "C'est un mur." << std::endl;
        return 1000;
    }
    if (!passage[dy][dx]) {
        std::cout << "On peut passer" << std::endl;
        passage[dy][dx] = 1;
        std::vector<int> moves;
        std::cout << "on essaie d'aller à droite" << std::endl;
        moves.push_back(countShortestWay(dx, dy+1, ax, ay, passage));
        std::cout << "on essaie d'aller à gauche" << std::endl;
        moves.push_back(countShortestWay(dx, dy-1, ax, ay, passage));
        std::cout << "on essaie d'aller en haut" << std::endl;
        moves.push_back(countShortestWay(dx-1, dy, ax, ay, passage));
        std::cout << "on essaie d'aller en bas" << std::endl;
        moves.push_back(countShortestWay(dx+1, dy, ax, ay, passage));
        return 1+min(moves);
    }
    std::cout << "On est déjà passés!" << std::endl;
    return 1000;
}

char GameManager::nextMove(float dx, float dy, float ax, float ay) {
 /*
    if (countShortestWay(dx, dy, ax, ay)) {
        std::cout << "Started : " << std::endl;
        std::vector<int> moves;
        if ((m_map->getStaticObjects()[dy][dx+1]->getType()!='W') && ((dx+1) >= 0 && (dx+1) < m_map->getNbX()) && (dx >= 0 && dx < m_map->getNbY())) {
            moves.push_back(countShortestWay(dx+1, dy, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy][dx-1]->getType()!='W') && ((dx-1) >= 0 && (dx-1) < m_map->getNbX()) && (dx >= 0 && dx < m_map->getNbY())) {
            moves.push_back(countShortestWay(dx-1, dy, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy+1][dx]->getType()!='W') && ((dx) >= 0 && (dx) < m_map->getNbX()) && ((dy-1) >= 0 && (dy-1) < m_map->getNbY())) {
            moves.push_back(countShortestWay(dx, dy+1, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy-1][dx]->getType()!='W') && ((dx) >= 0 && (dx) < m_map->getNbX()) && ((dy-1) >= 0 && (dy-1) < m_map->getNbY())) {
            moves.push_back(countShortestWay(dx, dy-1, ax, ay));
        }
        else moves.push_back(-1);
        int mini  = min(moves);
        switch(mini) {
            case 0: return 'D';
            case 1: return 'Q';
            case 2: return 'S';
            case 3: return 'Z';
        }
    }*/
    return 'N';
}

void GameManager::setTimers() {
    setFruitTimer(SDL_GetTicks());
    for (int i = 0; i < m_map->getGhosts().size(); i++) m_map->getGhosts()[i]->reset();
}

void GameManager::ghostMove() {

    Controller::Key action;
    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        if (m_map->getGhosts()[i]->ready()) {

            if ( (m_map->getGhosts()[i]->getPosY() == m_map->getGhosts()[i]->getInitY()) && (m_map->getGhosts()[i]->getPosX() == m_map->getGhosts()[i]->getInitX()) ) {
                m_map->getGhosts()[i]->setOrientation(Object::Orientation::UP);
                m_map->getGhosts()[i]->setPosX(m_map->getSpawnPoint()[0]->getPosX());
                m_map->getGhosts()[i]->setPosY(m_map->getSpawnPoint()[0]->getPosY());
            }
            else {

                switch (m_map->getGhosts()[i]->getOrientation()) {

                    case Object::Orientation::UP : action = Controller::Z;
                        break;
                    case Object::Orientation::DOWN: action = Controller::S;
                        break;
                    case Object::Orientation::RIGHT: action = Controller::D;
                        break;
                    case Object::Orientation::LEFT :action = Controller::Q;
                        break;
                }
                while (!moveCharacter(m_map->getGhosts()[i], action)) {

                    int r =  (rand() % 4);
                    switch (r) {

                        case 0: action = Controller::Z;
                            break;
                        case 1: action = Controller::Q;
                            break;
                        case 2: action = Controller::D;
                            break;
                        case 3:action = Controller::S;
                            break;
                    }
                }
            }
        }
    }
}

// Update speed with delta time
void GameManager::updateSpeed(uint32_t deltaTime)
{
    if(deltaTime > 0)
    {
        // Basic speed should be arround 0.02
        double speed = 0.004;
        m_map->getPacman()->setSpeed(speed*deltaTime);

        for (unsigned int i = 0; i < m_map->getGhosts().size(); ++i)
        {
            // If we can eat a ghost, slow down this ghost
            if(m_map->getGhosts()[i]->getSuper())
                m_map->getGhosts()[i]->setSpeed((speed*deltaTime)/1.5);
            else
                m_map->getGhosts()[i]->setSpeed(speed*deltaTime);
        }
    }
}

void GameManager::activateFruit() {

    int timer = 30000;  // 1sec * 1000
    if (SDL_GetTicks() - m_fruitTimer > timer)  {
        if (!m_map->getFruits().empty())
        {
            if (!m_map->getFruits()[0]->getAvailability() && m_map->getFruits()[0]->getFruit() != Edible::Fruit::NONE )
                m_map->getFruits()[0]->setAvailability(true);
        }
    }
}
