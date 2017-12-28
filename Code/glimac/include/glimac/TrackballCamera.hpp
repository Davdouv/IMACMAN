#pragma once

#include <glimac/glm.hpp>

#include "glimac/Camera.hpp"
#include "project/Controller.hpp"
#include "project/Character.hpp"

namespace glimac {

class TrackballCamera : public Camera {
public:
	TrackballCamera(float distance = 5.f, float hauteur = 0.f, float angleX = 0.f, float angleY = 0.f) :
	m_fDistance(distance), m_fHauteur(hauteur), m_fAngleX(angleX), m_fAngleY(angleY) {}

	~TrackballCamera() {}

    void moveFront(float delta); //permettant d'avancer / reculer la caméra. Lorsque delta est positif la caméra doit avancer, sinon elle doit reculer.
    void rotateLeft(float degrees); // permettant de tourner latéralement autour du centre de vision.
    void rotateUp(float degrees); // permettant de tourner verticalement autour du centre de vision.

	glm::mat4 getViewMatrix() const; // calcule la ViewMatrix de la caméra
	glm::mat4 getViewMatrix(Character* character, glm::vec2 gameSize) const; // calcule la ViewMatrix de la caméra

	void cameraController(Controller* controller);

private:
	float m_fDistance;		// Distance par rapport au centre de la scène
	float m_fHauteur;		// Hauteur de la caméra par rapport au centre de la scène
	float m_fAngleX;		// Angle effectuée par la caméra autour de l'axe X de la scène
	float m_fAngleY;		// Angle effectuée par la caméra autour de l'axe Y de la scène

	// Returns true if zoom is max/min, false if not
	bool zoomMax();
	bool zoomMin();
};

}