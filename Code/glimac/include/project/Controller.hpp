#pragma once

#include "glimac/SDLWindowManager.hpp"

namespace glimac {

class Controller {
public:
    enum Key {NONE, Z, Q, S, D, UP, LEFT, DOWN, RIGHT, C, ESCAPE};
    
public:
    Controller(SDLWindowManager* windowManager);

    void updateController();

    Key getPlayerAction();
    Key getCameraAction();
    Key getInterfaceAction();

private:
    Key playerAction;
    Key cameraAction;
    Key interfaceAction;
    SDLWindowManager* m_windowManager;

    void updatePlayerAction();
    void updateCameraAction();
    void updateInterfaceAction();
};

}
