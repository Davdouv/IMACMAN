#include "project/GameManager.hpp"
#include <string>
#include <iostream>
#include <random>
#include <cstdlib>

using namespace glimac;

static int once = 0;
GameManager::GameManager(Map* map)
{
    m_map = map;
    m_state = NORMAL;
    m_player.initialization();
    m_pause = false;
}

GameManager::PacmanState GameManager::getState() const { return m_state;}
uint32_t GameManager::getStartTime() const { return m_startTime; }
uint32_t GameManager::getSuperTimer() const { return m_superTimer; }
bool GameManager::isPause() { return m_pause; }
int GameManager::getEatenGhosts() const { return m_eatenGhosts; }
void GameManager::setState(PacmanState state) { m_state = state;}
void GameManager::setStartTime(uint32_t t) { m_startTime = t;}
void GameManager::setSuperTimer(uint32_t t) { m_superTimer = t;}
void GameManager::switchPause() { m_pause=!isPause();}
void GameManager::setEatenGhosts(int eatenGhosts) { m_eatenGhosts = eatenGhosts;}
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
    }
        
}

void GameManager::start() {

    setGhosts();
    play();
}

// For console only
void GameManager::play() {

    Controller * c;
    Pacman *p = m_map->getPacman();
    p->setSpeed(1);
    m_map->setPacman(*p);

    std::string line;
    this->setStartTime(SDL_GetTicks());
    setGhosts();
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
            if (line == "C") return;
            pacmanGhostCollision();
            pacmanEdibleCollision();
            ghostMove();
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


// need to call setGhosts before this function to set them ready 
void GameManager::play(Controller* controller) {

    if (!(lost()) && ready() && !isPause())
    {
        stateManager();
        activateFruit();
        pacmanMove(controller);
        ghostMove();
        pacmanGhostCollision();
        pacmanEdibleCollision();
        if(won())
        {
            newLevel(controller);
        }
        if(lost())
        {
            std::cout << "Player Score : " << m_player.getPoints() << std::endl;
        }
    }
    pause(controller);
}

void GameManager::newLevel(Controller* controller)
{
    controller->setPlayerPreviousAction(Controller::Key::Q);
    setState(NORMAL);
    m_map->initialization();
    setGhosts();
}

void GameManager::pacmanGhostCollision() {

    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        if (m_map->getPacman()->collision(m_map->getGhosts()[i])) {
            switch(this->getState()) {

                case GameManager::PacmanState::NORMAL :
                    m_player.loseLife();
                    if (m_player.getLife() != 0)
                    {
                        std::cout << m_player.getLife() << std::endl;
                        m_map->getPacman()->reset();
                        setStartTime(SDL_GetTicks());
                    }
                    for (int i = 0; i < m_map->getGhosts().size(); i++) {
                        m_map->getGhosts()[i]->reset();
                    }
                    std::cout << "Life lost. Life : " << m_player.getLife() << std::endl;
                    break;
                case GameManager::PacmanState::SUPER :
                    m_map->getGhosts()[i]->reset();
                    m_map->getGhosts()[i]->reset();
                    eatGhost();
                    break;
                default:
                    break;
            }
        }
    }
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
    float seuil = 0.005;

    switch(direction) {
        case 'Z':
            // Check if we are close to the left edge
            if(fposX - iposX <= seuil)
            {
                return wallCollisionUP(fposY, iposY, iposX, speed, character);
            }
            // Check if we are close to the right edge
            else if (iposX+1 - fposX <= seuil)
            {
                // Consider we're on the right edge
                return wallCollisionUP(fposY, iposY, iposX+1, speed, character);
            }
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
            else if (iposY+1 - fposY <= seuil)
            {
                return wallCollisionLEFT(fposX, iposY+1, iposX, speed, character);
            }
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

void GameManager::pacmanEdibleCollision() {
    // If we're going left, we want Pacman to be half inside the cell
    if ((m_map->getPacman()->getPosX() - (int)m_map->getPacman()->getPosX()) > m_map->getPacman()->getWidth())
        return;
    // If we're going up, we want Pacman to be half inside the cell
    if ((m_map->getPacman()->getPosY() - (int)m_map->getPacman()->getPosY()) > m_map->getPacman()->getHeight())
        return;
    if (m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->getType()=='E'){
        //std::cout << "we're here ! " << std::endl;
        Edible *e;
        e =  (Edible*) m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()];
        switch (e->getTypeEdible()) {

            case Edible::Type::FRUIT : 
                if (e->getAvailability()) {
                    std::cout << "Fruit caught" << std::endl;
                    m_player.gainPoints(e->gain());
                    e->setAvailability(false);
                    m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()] = e;
                }
                break;

            case Edible::Type::PAC_GOMME : std::cout << "Pac gomme caught! " << std::endl;
                m_player.gainPoints(e->gain());
                m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->setType('V');

                break;

            case Edible::Type::SUPER_PAC_GOMME : std::cout << " Super pac gomme caught" << std::endl;
                m_player.gainPoints(e->gain());
                switchSuperState();
                m_map->getStaticObjects()[m_map->getPacman()->getPosY()][m_map->getPacman()->getPosX()]->setType('V');
                break;
            default:return;
        }
        //std::cout << "Points : " << m_player.getPoints() << std::endl;
    }
}

void GameManager::switchSuperState() {
            
    this->setState(GameManager::PacmanState::SUPER);
    this->setSuperTimer(SDL_GetTicks());
    this->setEatenGhosts(0);
    for (int i = 0; i < m_map->getGhosts().size(); i++) { 
        m_map->getGhosts()[i]->setSuper(true);
        m_map->getGhosts()[i]->slowDown();
    }
}

void GameManager::stateManager() {

    if (this->getState() == GameManager::PacmanState::SUPER) {
        if (SDL_GetTicks() - this->getSuperTimer() > 60000) {
            this->setState(GameManager::PacmanState::NORMAL);
            this->setEatenGhosts(0);
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

int GameManager::countShortestWay(float dx, float dy, float ax, float ay) {
    
    return 0;
    /*
    if (dx == ax && dy == ay) return 0;
    else {
        std::vector<int> moves;
        if ((m_map->getStaticObjects()[dy][dx+1]->getType()!='W') && ((dx+1) >= 0 && (dx+1) < m_map->getNbX()) && (dx >= 0 && dx < m_map->getNbY())) {
            std::cout << "Added right " << dy << " , " << dx+1 << std::endl;
            moves.push_back(countShortestWay(dx+1, dy, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy][dx-1]->getType()!='W') && ((dx-1) >= 0 && (dx-1) < m_map->getNbX()) && (dx >= 0 && dx < m_map->getNbY())) {
            std::cout << "Added left" << std::endl;
            moves.push_back(countShortestWay(dx-1, dy, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy+1][dx]->getType()!='W') && ((dx) >= 0 && (dx) < m_map->getNbX()) && ((dy-1) >= 0 && (dy-1) < m_map->getNbY())) {
            std::cout << "Added down" << std::endl;
            moves.push_back(countShortestWay(dx, dy+1, ax, ay));
        }
        else moves.push_back(-1);
        if ((m_map->getStaticObjects()[dy-1][dx]->getType()!='W') && ((dx) >= 0 && (dx) < m_map->getNbX()) && ((dy-1) >= 0 && (dy-1) < m_map->getNbY())) {
            std::cout << "Added up" << std::endl;
            moves.push_back(countShortestWay(dx, dy-1, ax, ay));
        }
        else moves.push_back(-1);
        return 1+min(moves);
    }
    */
}

char GameManager::nextMove(float dx, float dy, float ax, float ay) {
    
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
    }
    return 'N';
}

void GameManager::setGhosts() {
    for (int i = 0; i < m_map->getGhosts().size(); i++) m_map->getGhosts()[i]->reset();
}

void GameManager::ghostMove() {

    Controller::Key action;
    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        if (m_map->getGhosts()[i]->ready()) {

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
            m_map->getGhosts()[i]->setSpeed(speed*deltaTime);
        }
    }
}

void GameManager::activateFruit() {

    // Every 30sec
    if ((SDL_GetTicks() - m_startTime) % 30000 == 0)  {
        if (!m_map->getFruits().empty())
        {
            std::cout << "Fruit available!" << m_map->getFruits().size() << std::endl;
            if (!m_map->getFruits()[0]->getAvailability()) m_map->getFruits()[0]->setAvailability(true);
        }
    }
}
