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

glm::mat4 TrackballCamera::getViewMatrix(Character* character, glm::vec2 gameSize) const
{	
	glm::mat4 MVMatrix;
	MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-(character->getPosX()+gameSize.y-3), -m_fHauteur, -(character->getPosY()+gameSize.x+m_fDistance)));
	// MVMatrix = glm::rotate(MVMatrix, m_fAngleX, glm::vec3(0, 1, 0));
	// MVMatrix = glm::rotate(MVMatrix, m_fAngleY, glm::vec3(1, 0, 0));
	// MVMatrix = glm::translate(MVMatrix, glm::vec3(-character->getPosY(), 0.f, 0.f));

	//MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-3.f, -3.f, -3.f));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleX, glm::vec3(0, 1, 0));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleY, glm::vec3(1, 0, 0));
	MVMatrix = glm::translate(MVMatrix, glm::vec3(0.f, +character->getPosY(), -(character->getPosY()+gameSize.x+3)));
	//std::cout << character->getPosY()+gameSize.x << std::endl;
	//(character->getPosY()-gameSize.x)
	return MVMatrix;
}

// Tells if we can zoom or not
bool TrackballCamera::zoomMax()
{
	if (m_fDistance <= 2.0)
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
void TrackballCamera::cameraController(Controller* controller)
{
	Controller::Key action = controller->getCameraAction();
	float speed = -0.05;

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