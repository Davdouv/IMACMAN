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
    m_pauseTime = 0;
    m_pauseStartTime = 0;
}

GameManager::PacmanState GameManager::getState() const { return m_state;}
uint32_t GameManager::getStartTime() const { return m_startTime; }
uint32_t GameManager::getPauseTime() const { return m_pauseTime; }
uint32_t GameManager::getPauseStartTime() const { return m_pauseStartTime; }
uint32_t GameManager::getSuperTimer() const { return m_superTimer; }
uint32_t GameManager::getFruitTimer() const { return m_fruitTimer; }
bool GameManager::isPause() { return m_pause; }
bool GameManager::isLost() { return m_lost; }
int GameManager::getEatenGhosts() const { return m_eatenGhosts; }
Player* GameManager::getPlayer() { return &m_player; }
Map* GameManager::getMap() { return m_map; }
void GameManager::setState(PacmanState state) { m_state = state;}
void GameManager::setStartTime(uint32_t t) { m_startTime = t;}
void GameManager::setPauseTime(uint32_t t) {
  if (m_pause == false && m_pauseTimeRecording == true)
  {
    m_pauseTime += t - m_pauseStartTime;
    m_pauseStartTime = 0;
    m_pauseTimeRecording = false;
  }
}
void GameManager::setPauseStartTime(uint32_t t) {
  if (m_pauseTimeRecording == false)
  {
    m_pauseStartTime = t;
    m_pauseTimeRecording = true;
  }
}

void GameManager::setSuperTimer(uint32_t t) { m_superTimer = t;}
void GameManager::setFruitTimer(uint32_t t) { m_fruitTimer = t; }
void GameManager::switchPause() { m_pause=!isPause(); }
void GameManager::setEatenGhosts(int eatenGhosts) { m_eatenGhosts = eatenGhosts;}

/* -------------
 *  MAIN METHOD
 * ------------- */

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
            std::cout << "Player Score : " << m_player.getPoints() << std::endl;
        }
    }
    pause(controller);
}

/* ---------------------
 *  FILE MANAGER
 * --------------------- */


/*  Game loader
 *  reads a txt file  
 *  set true to newGame if brand new Game wanted
 *  set false to newGame if last saved Game wanted
 */
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
 
        if (!newGame) {  // if saved game, load player lives and points
        /*** PLAYER INFORMATION ***/
            getline(file,tmp);
            m_player.setLife(atoi(tmp.c_str()));
            getline(file,tmp);
            m_player.setPoints(atoi(tmp.c_str()));
        }

        /*** PACMAN INFORMATION ***/
        getline(file,tmp);
        std::string delimiter = ",";
        std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
        std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        Pacman *p = new Pacman(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.5, 0.004, Object::Orientation::LEFT);
        
        if (!newGame) {  // if saved game get pacman initial positions
            getline(file,tmp);
            std::string delimiter = ",";
            std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
            std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
            p->setInitX(atoi(pos_x.c_str()));
            p->setInitY(atoi(pos_y.c_str()));
        }
        m_map->setPacman(*p);
        delete(p);
        
        /*** GHOSTS INFORMATIONS ***/
        std::vector<Ghost> tabGhost;
        for (int i = 0; i < 4; i++) {
            getline(file,tmp);
            std::string delimiter = ",";
            std::string pos_x = tmp.substr(1, tmp.find(delimiter)-1);
            std::string pos_y = tmp.substr(tmp.find(delimiter)+1, tmp.size());
            Ghost *g = new Ghost(atoi(pos_x.c_str()), atoi(pos_y.c_str()), 0.5, 0.75, 0.004, i, Object::Orientation::LEFT, 0);
            
            if (!newGame) { // if saved game get ghosts initial position
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

        /** MAP/STATIC OBJECTS **/
        bool once = true;
        while (!file.eof()) {
            getline(file, tmp);
            std::vector<StaticObject*> row;
            if(once)
            {
                m_map->setNbY(tmp.size()); // number of columns in the maze
                once = false;
            }
            for (int j = 0; j < tmp.size(); j++) {
                StaticObject *o;
                if (!isStaticElement(tmp[j])) tmp[j] = 'V'; // if error in the file replace it by an empty path
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
        m_map->setNbX(i-1); // number of rows in the maze
    }
    file.close();
    return 1;
}

/*  Game saver
 *  saves the game's information in a  txt file
 */
int GameManager::save() {

    std::fstream file;
    file.open("../Code/data/save_"+m_map->getFileMap(), std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::ofstream outfile("../Code/data/save_"+m_map->getFileMap()); // create new file if the game was never saved before
        outfile.close();
        save(); // recall the function to save the game in the file that was just created
    }
    else {
        file.seekg(0);
        
        /*** PLAYER INFORMATION ***/
        file << m_player.getLife() << std::endl;
        file << m_player.getPoints() << std::endl;

        /*** PACMAN INFORMATION ***/
        file << "P" << m_map->getPacman()->getPosX() << "," << m_map->getPacman()->getPosY() << std::endl;
        file << "P" << m_map->getPacman()->getInitX() << "," << m_map->getPacman()->getInitY() << std::endl;

        /*** GHOSTS INFORMATION ***/
        file << "G" << m_map->getGhosts()[Ghost::Type::SHADOW]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::SHADOW]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SHADOW]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::SHADOW]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SPEEDY]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::SPEEDY]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::SPEEDY]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::BASHFUL]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::BASHFUL]->getInitY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::POKEY]->getPosX() << "," << m_map->getGhosts()[Ghost::Type::POKEY]->getPosY() << std::endl;
        file << "G" << m_map->getGhosts()[Ghost::Type::POKEY]->getInitX() << "," << m_map->getGhosts()[Ghost::Type::POKEY]->getInitY() << std::endl;

        /*** MAP/STATIC OBJECTS ***/
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



/*  ---------------------
 *  BASIC OPTIONS MANAGER
 *  --------------------- */


/* Pause switcher
 * when the player hits ESCAPE 
 * its stops the game and the music
 */
void GameManager::pause(Controller* controller) {

    if (controller->getInterfaceAction() == Controller::ESCAPE)
    {
                switchPause();
                controller->setInterfaceAction(Controller::NONE);
                if (m_pause) Mix_PauseMusic();
                else Mix_ResumeMusic();
    }    
}

/* Game restarter
 * it initializes the player 
 * and loads a new game
 */
void GameManager::restart() {

    setState(NORMAL);
    m_map->setFileMap("classicMap.txt");
    load(true);
    m_map->initialization();
    m_player.initialization();
    setTimers();
    switchPause();
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


/*  -------------------------
 *  GAME/PLAYER STATE MANAGER
 *  ------------------------- */

/*  "Winner" detector
 *  if there are no edible on the map
 */
bool GameManager::won() {
    
    return (m_map->getSuperPacGommes().empty() && m_map->getPacGommes().empty());
}

/*  Loser  detector
 *  if the player lost all of his lives
 */
bool GameManager::lost() {

    return (!m_player.getLife());
}

/* Game timer
 * waits for 1 second before starting the game
 */
bool GameManager::ready() {

    return (SDL_GetTicks() - getStartTime() > 1000);
}

/* timers initialization
 * waits for 1 second before starting the game
 */
void GameManager::setTimers() {
    setFruitTimer(SDL_GetTicks());
    for (int i = 0; i < m_map->getGhosts().size(); i++) m_map->getGhosts()[i]->reset();
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
                m_map->getGhosts()[i]->setSpeed((speed*deltaTime)/2);
            else
                m_map->getGhosts()[i]->setSpeed(speed*deltaTime);
        }
    }
}

/*  --------------
 *  MOVES MANAGER
 *  -------------- */


/* Character's move ability
 * Returns true if can move, false if not (or if he takes door)
 */
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

/* Pacman moves
 * controlled by the player/controller
 */
void GameManager::pacmanMove(Controller* controller) {
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

/* Ghosts moves
 * the move around randomly in the maze
 * they keep going in the same direction until 
 * they collide to a wall
 * then they change direction randomly again
 * and take an availble path
 */
void GameManager::ghostMove() {

    Controller::Key action;
    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        if (m_map->getGhosts()[i]->ready()) {

            if ( (m_map->getGhosts()[i]->getPosY() == m_map->getGhosts()[i]->getInitY()) && (m_map->getGhosts()[i]->getPosX() == m_map->getGhosts()[i]->getInitX()) ) {
                m_map->getGhosts()[i]->setOrientation(Object::Orientation::UP);
                m_map->getGhosts()[i]->setPosX(m_map->getSpawnPoint()[0]->getPosX());
                m_map->getGhosts()[i]->setPosY(m_map->getSpawnPoint()[0]->getPosY()-1);
                m_map->getGhosts()[i]->setInSpawn(false);
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


/*  --------------
 *  COLLISIONS MANAGER
 *  -------------- */

/* Detects a collision between Pacman and a Ghost
*/
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


// Detects a collision between a Character and the upper side of a wall
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
            else if (!character->inSpawn() && m_map->getStaticObjects()[iposY-1][iposX]->getType()=='X')
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

// Detects a collision between a Character and the left side of a wall
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
            else if (!character->inSpawn() && m_map->getStaticObjects()[iposY][iposX-1]->getType()=='X')
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

// Detects a collision between a Character and the down side of a wall
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
        else if (!character->inSpawn() && m_map->getStaticObjects()[iposY+1][iposX]->getType()=='X')
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

// Detects a collision between a Character and the right side of a wall
bool GameManager::wallCollisionRIGHT(float fposX, int iposY, int iposX, float speed, Character* character)
{
    if(iposX+1 <= m_map->getNbY()-1)
    {
        if (m_map->getStaticObjects()[iposY][iposX+1]->getType()=='W')
        {
            character->setPosX((float)iposX);
            return true;
        }
        else if (!character->inSpawn() && m_map->getStaticObjects()[iposY][iposX+1]->getType()=='X')
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

// Detects a collision between a Character and a Wall
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

// Detects a collision between a Character and the left Door
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

// Detects a collision between a Character and the right
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
/* Note that doors must be placed on the left and right sides
 * of the maze in order for teleportation to work 
 */

/* Detects a Collision between an Edible object and Pacman
 * points gaining
 * fruit upgrade if fruit
 * switch to super mode if super pac gomme
 */
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

/*  --------------
 *  STATE MANAGER
 *  -------------- */

/* Switch to super state mode
 * it includes : 
 * pacman's ability to eat ghost
 * eaten ghosts counter initialization
 */
void GameManager::switchSuperState() {
    this->setState(GameManager::PacmanState::SUPER);
    this->setSuperTimer(SDL_GetTicks());
    this->setEatenGhosts(0);
    for (int i = 0; i < m_map->getGhosts().size(); i++) {
        m_map->getGhosts()[i]->setSuper(true);
    }
}

/* if the super mode is on
 * set the time (7 seconds)
 * go back to normal state when time's up
 */
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


/*  --------------
 *  EDIBLE MANAGER
 *  -------------- */

/* while the super mode is on
 * double the gained points everytime 
 * a ghost is eaten
 */
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

/* fruit activation
 * a fruit is availble 30 seconds after the game starts
 * until pacman eats it
 * then 30 seconds later another one appears
 */
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