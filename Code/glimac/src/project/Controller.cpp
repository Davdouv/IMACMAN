#include "project/Controller.hpp"
#include <glimac/glm.hpp>
#include <iostream>

namespace glimac {

// Constructor
Controller::Controller(SDLWindowManager* windowManager)
{
    m_windowManager = windowManager;
    playerAction = Q;
	previousPlayerAction = Q;
    cameraAction = NONE;
    interfaceAction = NONE;
	m_mousePosition = windowManager->getMousePosition();
	FPS = false;
}

// Tells if the mouse goes Up (true) or Not (false)
bool Controller::isMouseUp()
{
	glm::vec2 currentPosition = m_windowManager->getMousePosition();
	if (currentPosition.y > m_mousePosition.y)
	{
		m_mousePosition = currentPosition;
		return true;
	}
	else
		return false;
}
// Tells if the mouse goes Down (true) or Not (false)
bool Controller::isMouseDown()
{
	glm::vec2 currentPosition = m_windowManager->getMousePosition();
	if (m_windowManager->getMousePosition().y < m_mousePosition.y)
	{
		m_mousePosition = currentPosition;
		return true;
	}
	else
		return false;
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
	// If Right Click
	if (m_windowManager->isMouseButtonPressed(SDL_BUTTON_RIGHT))
    {
		if (isMouseUp())
		{
			cameraAction = DOWN;
		}
		else if (isMouseDown())
		{
			cameraAction = UP;
		}
	}
	// Else check keys
    else if (m_windowManager->isKeyPressed(SDLK_UP))
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
		if(FPS)
			FPS = false;
		else
			FPS = true;
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
Controller::Key Controller::getPlayerAction() const
{
    return playerAction;
}
Controller::Key Controller::getCameraAction() const
{
    return cameraAction;
}
Controller::Key Controller::getInterfaceAction() const
{
    return interfaceAction;
}
bool Controller::isFPSactive()
{
	return FPS;
}
Controller::Key Controller::getPlayerPreviousAction() const
{
    return previousPlayerAction;
}

// Setters
void Controller::setInterfaceAction(Key key)
{
	interfaceAction = key;
}
void Controller::setFPS(bool fps)
{
	FPS = fps;
}
void Controller::switchFPS()
{
	std::cout << FPS << std::endl;
	if(FPS)
		FPS = false;
	else
		FPS = true;
}
void Controller::setPlayerPreviousAction(Key key)
{
	previousPlayerAction = key;
}

}