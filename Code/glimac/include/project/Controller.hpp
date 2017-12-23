#pragma once

#include "glimac/SDLWindowManager.hpp"

namespace glimac {

class Controller {
public:
    // Every key player can use
    enum Key {NONE, Z, Q, S, D, UP, LEFT, DOWN, RIGHT, C, ESCAPE};
    
public:
    // Constructor
    Controller(SDLWindowManager* windowManager);

    // Update function, called each frame
    void updateController();

    // Getters
    Key getPlayerAction() const;
    Key getCameraAction() const;
    Key getInterfaceAction() const;

private:
    // 3 kind of interaction : Player (PacMan movements), Camera (Camera movements), Interface (Pause button)
    Key playerAction;
    Key cameraAction;
    Key interfaceAction;
    // Pointer on the windowManager to get the events
    SDLWindowManager* m_windowManager;
    // Store mouse position
    glm::ivec2 m_mousePosition;

    // Tell if the mouse goes Up or Down
    bool isMouseUp();
    bool isMouseDown();
    // Update Actions, called each frame
    void updatePlayerAction();
    void updateCameraAction();
    void updateInterfaceAction();
};

}
