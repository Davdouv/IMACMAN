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
void Controller::updatePlayerAction(Pacman* pacman, SDL_Event& e)
{
	bool z = false;
	bool q = false;
	bool s = false;
	bool d = false;

	if (e.type == SDL_JOYHATMOTION)
	{
		if (e.jhat.hat == 0)
		{
			switch(e.jhat.value)
			{
				case SDL_HAT_UP:
					playerAction = Z;
					if(!FPS) return;
					z = true;
					break;
				case SDL_HAT_DOWN:
					playerAction = S;
					if(!FPS) return;
					s = true;
					break;
				case SDL_HAT_LEFT:
					playerAction = Q;
					if(!FPS) return;
					q = true;
					break;
				case SDL_HAT_RIGHT:
					playerAction = D;
					if(!FPS) return;
					d = true;
					break;
				default:
					break;
			}
		}
	}

	if (m_windowManager->isKeyPressed(SDLK_z))
	{
		playerAction = Z;
		z = true;
	}
	else if (m_windowManager->isKeyPressed(SDLK_q))
	{
		playerAction = Q;
		q = true;
	}	
	else if (m_windowManager->isKeyPressed(SDLK_s))
	{
		playerAction = S;
		s = true;
	}
	else if (m_windowManager->isKeyPressed(SDLK_d))
	{
		playerAction = D;
		d = true;
	}	
	// If we are in FPS mode, we look always forward so Keys must changes
	if(FPS)
	{
		switch(pacman->getOrientation())
		{
			case Object::Orientation::UP:
				if (z)
					playerAction = Z;
				else if (q)
					playerAction = Q;
				else if (s)
					playerAction = S;
				else if (d)
					playerAction = D;
				break;
			case Object::Orientation::LEFT:
				if (z)
					playerAction = Q;
				else if (q)
					playerAction = S;
				else if (s)
					playerAction = D;
				else if (d)
					playerAction = Z;
				break;
			case Object::Orientation::RIGHT:
				if (z)
					playerAction = D;
				else if (q)
					playerAction = Z;
				else if (s)
					playerAction = Q;
				else if (d)
					playerAction = S;
				break;
			case Object::Orientation::DOWN:
				if (z)
					playerAction = S;
				else if (q)
					playerAction = D;
				else if (s)
					playerAction = Z;
				else if (d)
					playerAction = Q;
				break;
			default:
				break;
		}
	}
}

// Update Camera Action Key
void Controller::updateCameraAction(SDL_Event& e)
{
	if(e.type == SDL_JOYAXISMOTION)
	{
		if (e.jaxis.axis == 5)	// LT
		{
			if (e.jaxis.value > -20000)
			{
				cameraAction = UP;
				return;
			}
		}
		if (e.jaxis.axis == 2)	// RT
		{
			if (e.jaxis.value > -20000)
			{
				cameraAction = DOWN;
				return;
			}
		}
	}
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

    else
    {
        cameraAction = NONE;
    }
}

// Update Interface Action Key
void Controller::updateInterfaceAction(SDL_Event& e)
{
	if (e.type == SDL_JOYBUTTONDOWN)
	{
			switch(e.jbutton.button)
			{
				case 0 :	// ENTER
					interfaceAction = ENTER;
					return;
				case 2 :	// CAMERA SWITCH
					interfaceAction = C;
					return;
				case 3 :	// PAUSE
					interfaceAction = ESCAPE;
					return;
				default:
					break;
			}
	}
	else if (e.type == SDL_JOYHATMOTION)
	{
		if (e.jhat.hat == 0)
		{
			switch(e.jhat.value)
			{
				case SDL_HAT_UP:
					interfaceAction = UP;
					return;
				case SDL_HAT_DOWN:
					interfaceAction = DOWN;
					return;
				default:
					break;
			}
		}
	}
    if (m_windowManager->isKeyPressed(SDLK_ESCAPE))
	{
		interfaceAction = ESCAPE;
	}
	else if (m_windowManager->isKeyPressed(SDLK_c))
	{
		interfaceAction = C;
		FPS =! FPS;
	}
	else if (m_windowManager->isKeyPressed(SDLK_UP))
	{
		interfaceAction = UP;
	}
	else if (m_windowManager->isKeyPressed(SDLK_DOWN))
	{
		interfaceAction = DOWN;
	}
	else if (m_windowManager->isKeyPressed(SDLK_LEFT))
	{
		interfaceAction = LEFT;
	}
	else if (m_windowManager->isKeyPressed(SDLK_RIGHT))
	{
		interfaceAction = RIGHT;
	}
	else if (m_windowManager->isKeyPressed(SDLK_RETURN))
	{
		interfaceAction = ENTER;
	}
    else
    {
        interfaceAction = NONE;
    }
}

// Call all the updates functions
void Controller::updateController(Pacman* pacman, SDL_Event& e)
{
    updatePlayerAction(pacman, e);
    updateCameraAction(e);
    updateInterfaceAction(e);
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
void Controller::setPlayerAction(Key key)
{
	playerAction = key;
}

// If we are in FPS mode, we go forward but keys have other interpretations
/*
Controller::Key Controller::getFPSkey(Pacman* pacman)
{
	Key action;
    switch(pacman->getOrientation())
    {
        case Object::Orientation::LEFT:
            if (m_windowManager->isKeyPressed(SDLK_z))
                action = Key::Q;
            else if (m_windowManager->isKeyPressed(SDLK_q))
                action = Key::S;
            else if (m_windowManager->isKeyPressed(SDLK_s))
                action = Key::D;
            else if (m_windowManager->isKeyPressed(SDLK_d))
                action = Key::Z;
            break;
        case Object::Orientation::RIGHT:
            if (m_windowManager->isKeyPressed(SDLK_z))
                action = Key::D;
            else if (m_windowManager->isKeyPressed(SDLK_q))
                action = Key::Z;
            else if (m_windowManager->isKeyPressed(SDLK_s))
                action = Key::Q;
            else if (m_windowManager->isKeyPressed(SDLK_d))
                action = Key::S;
            break;
        case Object::Orientation::DOWN:
            if (m_windowManager->isKeyPressed(SDLK_z))
                action = Key::S;
            else if (m_windowManager->isKeyPressed(SDLK_q))
                action = Key::D;
            else if (m_windowManager->isKeyPressed(SDLK_s))
                action = Key::Z;
            else if (m_windowManager->isKeyPressed(SDLK_d))
                action = Key::Q;
            break;
        default:
            break;
    }

    return action;
}*/

void Controller::joystickController(SDL_Event& e)
{

}

}