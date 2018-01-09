#pragma once
#include "project/Map.hpp"
#include "Player.hpp"
#include "project/AudioManager.hpp"

using namespace glimac;

class GameManager {

public:
    enum PacmanState { NORMAL, SUPER };

private:

    // Game elements and state
    Map* m_map;
    PacmanState m_state;    // super or normal mode, also the map state
    Player m_player;

    // timers
    uint32_t m_startTime;
    uint32_t m_pauseTime;
    uint32_t m_pauseStartTime;
    uint32_t m_totalTime;
    uint32_t m_superTimer;
    uint32_t m_fruitTimer;

    // true is the game is paused

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
    Uint32 getTotalTime() const;
    bool isPause();
    bool isLost();
    int getEatenGhosts() const;
    Player* getPlayer();
    Map* getMap();

    // setters
    void setState(PacmanState);
    void setStartTime(uint32_t);
    void setPauseTime(uint32_t);
    void setTotalTime();
    void setPauseStartTime(uint32_t);
    void setSuperTimer(uint32_t);
    void setFruitTimer(uint32_t);
    void switchPause();
    void setEatenGhosts(int);

    void start(AudioManager*);
    void play(Controller* controller, AudioManager* audioManager);
    void restart();
    void setTimers();

    // Use of Delta Time
    void updateSpeed(uint32_t deltaTime);

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

    // New game
    void newLevel(Controller*);

    // super state
    void switchSuperState();
    void stateManager();
    void eatGhost();

    void activateFruit();
};
