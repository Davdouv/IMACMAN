#pragma once
#include "project/Map.hpp"
#include "Player.hpp"

using namespace glimac;

class GameManager {

public:
    enum PacmanState { NORMAL, SUPER };

private:
    Map* m_map;
    PacmanState m_state;    // If pacman can eat ghosts or not
    Player m_player;
    uint32_t m_startTime;

public:
    // constructor 
    GameManager(Map* map);

    // getters
    PacmanState getState() const;
    uint32_t getStartTime() const;

    // setters
    void setState(PacmanState);
    void setStartTime(uint32_t);
    
    void play();
    void play(Controller* controller);

private:

    // game state
    bool won();
    bool lost();
    bool ready();
    // Move
    bool moveCharacter(Character*, Controller::Key);
    void pacmanMove(Controller* controller);
    void ghostMove();

    // collisions
    bool wallCollisionUP(float fposY, int iposY, int iposX, float speed, Character* character);
    bool wallCollisionLEFT(float fposX, int iposY, int iposX, float speed, Character* character);
    bool wallCollisionDOWN(float fposY, int iposY, int iposX, float speed, Character* character);
    bool wallCollisionRIGHT(float fposX, int iposY, int iposX, float speed, Character* character);
    bool characterWallCollision(Character*, char);

    bool characterLeftDoorCollision(Character*);
    bool characterRightDoorCollision(Character*);

    void pacmanGhostCollision();
    bool ghostCollision();
    void pacmanEdibleCollision();

    // Ghost moves algorithms
    int shortestWay(int, float, float);
    void shadowAI();
    void speedyAI();
    void bashfulAI();
    void pokeyAI();    
    char nextMove(float, float, float, float);
    int countShortestWay(float, float, float, float);

    void setGhosts();
    // New game
    void newLevel();
};