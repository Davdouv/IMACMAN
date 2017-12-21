#include "project/Controller.hpp"
#include <iostream>

namespace glimac {

// Constructor
Controller::Controller(SDLWindowManager* windowManager)
{
    m_windowManager = windowManager;
    playerAction = NONE;
    cameraAction = NONE;
    interfaceAction = NONE;
}

// Update Player Action Key
void Controller::updatePlayerAction()
{
    if (m_windowManager->isKeyPressed(SDLK_z))
	{
		playerAction = Z;
	}
	else if (m_windowManager->isKeyPressed(SDLK_q))
	{
		playerAction = Q;
	}
	else if (m_windowManager->isKeyPressed(SDLK_s))
	{
		playerAction = S;
	}
	else if (m_windowManager->isKeyPressed(SDLK_d))
	{
		playerAction = D;
	}
}

// Update Camera Action Key
void Controller::updateCameraAction()
{
    if (m_windowManager->isKeyPressed(SDLK_UP))
	{
		cameraAction = UP;
	}
	else if (m_windowManager->isKeyPressed(SDLK_DOWN))
	{
		cameraAction = DOWN;
	}
	else if (m_windowManager->isKeyPressed(SDLK_LEFT))
	{
		cameraAction = LEFT;
	}
	else if (m_windowManager->isKeyPressed(SDLK_RIGHT))
	{
		cameraAction = RIGHT;
	}
    else
    {
        cameraAction = NONE;
    }
}

// Update Interface Action Key
void Controller::updateInterfaceAction()
{
    if (m_windowManager->isKeyPressed(SDLK_ESCAPE))
	{
		interfaceAction = ESCAPE;
	}
	else if (m_windowManager->isKeyPressed(SDLK_c))
	{
		interfaceAction = C;
	}
    else
    {
        interfaceAction = NONE;
    }
}

// Call all the updates functions
void Controller::updateController()
{
    updatePlayerAction();
    updateCameraAction();
    updateInterfaceAction();
}

// Getters
Controller::Key Controller::getPlayerAction()
{
    return playerAction;
}
Controller::Key Controller::getCameraAction()
{
    return cameraAction;
}
Controller::Key Controller::getInterfaceAction()
{
    return interfaceAction;
}

}