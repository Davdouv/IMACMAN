#include "glimac/TrackballCamera.hpp"

#include <GL/glew.h>
#include <glimac/glm.hpp>
#include <iostream>

using namespace glimac;

// Move the camera towards
void TrackballCamera::moveFront(float delta)
{
	m_fDistance += delta;
}
// Rotate the camera on the X axis
void TrackballCamera::rotateLeft(float degrees)
{
	m_fAngleX += degrees;
}
// Rotate the camera on the Y axis
void TrackballCamera::rotateUp(float degrees)
{
	m_fAngleY += degrees;
}

// Returns the ViewMatrix of the camera, called each frame
glm::mat4 TrackballCamera::getViewMatrix() const
{
	// MODIFIER CETTE LIGNE POUR FOCUS SUR PACMAN : 1er parametre du glm::vec3
	glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -m_fHauteur, -m_fDistance));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleX, glm::vec3(0, 1, 0));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleY, glm::vec3(1, 0, 0));

	return MVMatrix;
}

glm::mat4 TrackballCamera::getViewMatrix(glm::mat4 matrix) const
{
	// MODIFIER CETTE LIGNE POUR FOCUS SUR PACMAN : 1er parametre du glm::vec3
	
	glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -m_fHauteur, -m_fDistance));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleX, glm::vec3(0, 1, 0));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleY, glm::vec3(1, 0, 0));

	return MVMatrix;
}

// This function was made especially for Pacman Game
glm::mat4 TrackballCamera::getViewMatrix(Character* character, glm::vec2 gameSize) const
{	
	// Values I tried to center Pacman
	float var1 = 3;
	float var2 = 19;

	glm::mat4 MVMatrix;
	// First we rotate the camera. m_fAngleY must be between 0 (Upper view) and -1.57 (Frontal view)
	MVMatrix = glm::rotate(glm::mat4(1.f), m_fAngleY, glm::vec3(1, 0, 0));
	// Then we set the X and Z position of the camera to be placed ON pacman, and the Y position was just guessed magicaly
	MVMatrix = glm::translate(MVMatrix, glm::vec3(-(character->getPosX()+gameSize.y-var1), 
	 							-m_fHauteur + (m_fDistance - var2)/2, -(character->getPosY()+gameSize.x+m_fDistance)));
	// Then we rotate the cam 90° so it will have the good rotation
	MVMatrix = glm::rotate(MVMatrix, 1.57f, glm::vec3(1, 0, 0));
	// Finally, we do some translation to cancel the Zoom effect
	MVMatrix = glm::translate(MVMatrix, glm::vec3(0.f, +character->getPosY(), -(character->getPosY()+gameSize.x+var1)));
	return MVMatrix;
}

// Tells if we can zoom or not
bool TrackballCamera::zoomMax()
{
	if (m_fDistance <= 25.0)
		return true;
	else
		return false;
}
bool TrackballCamera::zoomMin()
{
	if (m_fDistance >= 50.0)
		return true;
	else
		return false;
}

// Control the camera
void TrackballCamera::cameraController(Controller* controller, uint32_t deltaTime)
{
	Controller::Key action = controller->getCameraAction();
	float speed = -0.05*deltaTime;

	switch (action)
	{
		case Controller::UP : 
			if(!zoomMax())
				this->moveFront(speed);
			break;
		case Controller::DOWN : 
			if(!zoomMin())
				this->moveFront(-speed);
			break;
		// case Controller::UP : 
		// 	this->rotateUp(speed);
		// 	break;
		// case Controller::DOWN : 
		// 	this->rotateUp(-speed);
		// 	std::cout << m_fAngleY << std::endl;
		// 	break;
		// case Controller::LEFT : 
		// 	this->rotateLeft(speed);
		// 	break;
		// case Controller::RIGHT : 
		// 	this->rotateLeft(-speed);
		// 	break;
		default :
			break;
	}
}