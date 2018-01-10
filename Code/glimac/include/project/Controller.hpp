#pragma once

#include "glimac/SDLWindowManager.hpp"
#include "project/Pacman.hpp"

namespace glimac {

class Controller {
public:
    // Every key player can use
    enum Key {NONE, Z, Q, S, D, UP, LEFT, DOWN, RIGHT, C, ESCAPE, ENTER, H};
    
public:
    // Constructor
    Controller(SDLWindowManager* windowManager);

    // Update function, called each frame
    void updateController(Pacman* pacman, SDL_Event& e);
    void updateInterfaceAction(SDL_Event& e);
    void switchFPS();

    // Joystick Controller
    void joystickController(SDL_Event& e);

    // Getters
    Key getPlayerAction() const;
    Key getPlayerPreviousAction() const;
    Key getCameraAction() const;
    Key getInterfaceAction() const;
    bool isFPSactive();

    // Setters
    void setPlayerAction(Key);
    void setPlayerPreviousAction(Key);
    void setInterfaceAction(Key);
    void setFPS(bool);

private:
    // 3 kind of interaction : Player (PacMan movements), Camera (Camera movements), Interface (Pause button)
    Key playerAction;
    Key previousPlayerAction;
    Key cameraAction;
    Key interfaceAction;
    // Pointer on the windowManager to get the events
    SDLWindowManager* m_windowManager;
    // Store mouse position
    glm::ivec2 m_mousePosition;
    bool FPS;

    // Tell if the mouse goes Up or Down
    bool isMouseUp();
    bool isMouseDown();
    // Update Actions, called each frame
    void updatePlayerAction(Pacman* pacman, SDL_Event& e);
    void updateCameraAction(SDL_Event& e);

    // Convert FPS key in TPS key
    Controller::Key getFPSkey(Pacman* pacman);
};

}
