#include "glimac/FreeflyCamera.hpp"

#include <GL/glew.h>
#include <glimac/glm.hpp>
#include <iostream>

using namespace glimac;

void FreeflyCamera::computeDirectionVectors()
{
	//std::cout << "Phi : " << m_fPhi << " Theta : " << m_fTheta << std::endl;

	//F⃗ =(cos(θ)sin(ϕ), sin(θ), cos(θ)cos(ϕ));
	m_FrontVector = glm::vec3( glm::cos(m_fTheta)*glm::sin(m_fPhi), glm::sin(m_fTheta), glm::cos(m_fTheta)*glm::cos(m_fPhi) );

	//L⃗ =(sin(ϕ+π/2), 0, cos(ϕ+π2))
	m_LeftVector = glm::vec3( glm::sin(m_fPhi + glm::pi<float>()/2), 0, glm::cos(m_fPhi + glm::pi<float>()/2) );

	//U⃗ =F⃗ × L⃗ 
	m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

// Constructeurs
FreeflyCamera::FreeflyCamera()
{
	m_Position = glm::vec3(0,0,0);
	m_fPhi = 270 * glm::pi<float>()/180;
	m_fTheta = 0.f;
	computeDirectionVectors();

	m_prevOrientation = Object::Orientation::LEFT;
	m_prevPhi = m_fPhi;
}

FreeflyCamera::FreeflyCamera(glm::vec3 position = glm::vec3(0), float phi = glm::pi<float>(), float theta = 0.f)
{
	m_Position = position;
	m_fPhi = phi;
	m_fTheta = theta;
	computeDirectionVectors();

	m_prevOrientation = Object::Orientation::LEFT;
	m_prevPhi = m_fPhi;
}

void FreeflyCamera::moveLeft(float t)
{
	//std::cout << m_Position << std::endl;
	m_Position += t*m_LeftVector;
	computeDirectionVectors();
}

void FreeflyCamera::moveFront(float t)
{
	//std::cout << m_Position << std::endl;
	m_Position += t*m_FrontVector;
	computeDirectionVectors();
}

void FreeflyCamera::rotateLeft(float degrees)
{
	m_fPhi += glm::radians(degrees);
	computeDirectionVectors();
}
void FreeflyCamera::rotateUp(float degrees)
{
	m_fTheta += glm::radians(degrees);
	computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
	glm::mat4 MVMatrix = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);

	return MVMatrix;
}

glm::mat4 FreeflyCamera::getViewMatrix(glm::mat4 matrix) const
{
	glm::mat4 MVMatrix = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);

	return MVMatrix;
}

glm::mat4 FreeflyCamera::getViewMatrix(Character* character, glm::vec2 gameSize) const
{
	glm::mat4 MVMatrix = glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);

	return MVMatrix;
}

void FreeflyCamera::interpolate(Character* character, uint32_t deltaTime)
{
	// If we change angle
	if (m_fPhi != (float)character->getOrientation() * glm::pi<float>()/180)
	{
		float speed = 0.01 * deltaTime;
		//std::cout << "Change angle " << std::endl;
		int dif = m_prevOrientation - character->getOrientation();
		// LEFT TO RIGHT
		if (dif == 90)
		{
			if (m_prevPhi > (float)character->getOrientation() * glm::pi<float>()/180)
				m_prevPhi -= speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
		// LEFT TO RIGHT but when we're are facing down
		else if (dif == -270)
		{
			if (m_prevPhi > (-90 * glm::pi<float>()/180))
				m_prevPhi -= speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
		// RIGHT TO LEFT
		else if (dif == -90)
		{
			if (m_prevPhi < (float)character->getOrientation() * glm::pi<float>()/180)
				m_prevPhi += speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
		// RIGHT TO LEFT but when we're facing left
		else if (dif == 270)
		{
			if (m_prevPhi < (360 * glm::pi<float>()/180))
				m_prevPhi += speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
		
		// Back direction
		else if (dif == 180)
		{
			if (m_prevPhi > (character->getOrientation() * glm::pi<float>()/180))
				m_prevPhi -= 2*speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
		else
		{
			if (m_prevPhi < (character->getOrientation() * glm::pi<float>()/180))
				m_prevPhi += 2*speed;
			else
			{
				m_prevOrientation = character->getOrientation();
				m_prevPhi = (float)character->getOrientation() * glm::pi<float>()/180;
			}
		}
	}
}

void FreeflyCamera::setCameraOnCharacter(Character* character, glm::vec2 gameSize, uint32_t deltaTime)
{
	float x, y;
	switch(character->getOrientation())
	{
		case Object::Orientation::UP:
			x = 0;
			y = 0.35;
			break;
		case Object::Orientation::LEFT:
			x = 0.75;
			y = -0.5;
			break;
		case Object::Orientation::DOWN:
			x = 0;
			y = -1;
			break;
		case Object::Orientation::RIGHT:
			x = -0.75;
			y = -0.5;
			break;
		default:
			x = 0;
			y = 0;
			break;
	}
	m_Position = glm::vec3(character->getPosX()-(gameSize.x/2)+x, 0.5, character->getPosY()-(gameSize.y/2)+character->getHeight()+y);

	interpolate(character, deltaTime);
		
	m_fPhi = m_prevPhi;

	//m_fTheta = 0.f;
	computeDirectionVectors();
}