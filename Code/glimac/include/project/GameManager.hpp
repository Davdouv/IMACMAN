#pragma once
#include "project/Map.hpp"
#include "Player.hpp"
#include "project/AudioManager.hpp"

using namespace glimac;

class GameManager {

public:
    enum PacmanState { NORMAL, SUPER };

private:
    Map* m_map;
    PacmanState m_state;    // If pacman can eat ghosts or not
    Player m_player;
    uint32_t m_startTime;
    uint32_t m_pauseTime;
    uint32_t m_pauseStartTime;
    uint32_t m_superTimer;
    uint32_t m_fruitTimer;
    bool m_pause;
    bool m_lost;
    bool m_pauseTimeRecording;
    int m_eatenGhosts;

public:
    // constructor
    GameManager(Map* map);

    // getters
    PacmanState getState() const;
    uint32_t getStartTime() const;
    uint32_t getPauseTime() const;
    uint32_t getPauseStartTime() const;
    uint32_t getSuperTimer() const;
    uint32_t getFruitTimer() const;
    bool isPause();
    bool isLost();
    int getEatenGhosts() const;
    Player* getPlayer();
    Map* getMap();

    // setters
    void setState(PacmanState);
    void setStartTime(uint32_t);
    void setPauseTime(uint32_t);
    void setPauseStartTime(uint32_t);
    void setSuperTimer(uint32_t);
    void setFruitTimer(uint32_t);
    void switchPause();
    void setEatenGhosts(int);

    void start(AudioManager*);
    void play(AudioManager*);
    void play(Controller* controller, AudioManager* audioManager);

    void setTimers();

    // Use of Delta Time
    void updateSpeed(uint32_t deltaTime);
    int countShortestWay(int, int, int, int, std::vector<std::vector<int>>);

    // file manager
    int load(bool);
    int save();

    // game state
    bool won();
    bool lost();
    bool ready();
    void pause(Controller* controller);

private:
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

    bool pacmanGhostCollision(AudioManager*);
    bool ghostCollision();
    bool pacmanEdibleCollision(AudioManager*);

    // Ghost moves algorithms
    Controller::Key shortestWay(int, float, float);
    void shadowAI();
    void speedyAI();
    void bashfulAI();
    void pokeyAI();
    char nextMove(float, float, float, float);

    // New game
    void newLevel(Controller*);

    // super state
    void switchSuperState();
    void stateManager();
    void eatGhost();

    void activateFruit();
};
