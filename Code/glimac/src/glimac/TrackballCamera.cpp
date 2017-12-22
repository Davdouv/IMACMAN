#include "glimac/TrackballCamera.hpp"

#include <GL/glew.h>
#include <glimac/glm.hpp>
#include <iostream>

using namespace glimac;

void TrackballCamera::moveFront(float delta)
{
	m_fDistance += delta;
}
void TrackballCamera::rotateLeft(float degrees)
{
	m_fAngleX += degrees;
}
void TrackballCamera::rotateUp(float degrees)
{
	m_fAngleY += degrees;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
	// MODIFIER CETTE LIGNE POUR ZOOMER SUR PACMAN : 1er parametre du glm::vec3
	glm::mat4 MVMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -m_fHauteur, -m_fDistance));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleX, glm::vec3(0, 1, 0));
	MVMatrix = glm::rotate(MVMatrix, m_fAngleY, glm::vec3(1, 0, 0));

	return MVMatrix;
}

void TrackballCamera::cameraController(Controller* controller)
{
	Controller::Key action = controller->getCameraAction();
	float speed = -0.005;

	switch (action)
	{
		case Controller::UP : 
			this->moveFront(speed);
			break;
		case Controller::DOWN : 
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