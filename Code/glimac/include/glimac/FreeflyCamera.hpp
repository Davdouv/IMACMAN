#pragma once

#include <glimac/glm.hpp>
#include "glimac/Camera.hpp"
#include "project/Character.hpp"

namespace glimac {

class FreeflyCamera : public Camera {
public:
	FreeflyCamera();
	FreeflyCamera(glm::vec3 position, float phi, float theta);

	~FreeflyCamera() {}

	void moveLeft(float t);
    void moveFront(float t);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

	glm::mat4 getViewMatrix() const; // calcule la ViewMatrix de la caméra
	glm::mat4 getViewMatrix(Character*) const; // calcule la ViewMatrix de la caméra

	// Set the camera on the camera to have a FPS view
	void setCameraOnCharacter(Character* character, glm::vec2 gameSize);

private:
	glm::vec3 m_Position;		// la position de la caméra
	float m_fPhi; 				// coordonnées sphériques du vecteur F
	float m_fTheta;				// coordonnées sphériques du vecteur F
	glm::vec3 m_FrontVector;	// vecteur F
	glm::vec3 m_LeftVector;		// vecteur L
	glm::vec3 m_UpVector;		// vecteur U

	void computeDirectionVectors();	// calcule les vecteurs F, L, U à partir des coordonnées spheriques
};

}
