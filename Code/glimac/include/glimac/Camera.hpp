#pragma once

#include <glimac/glm.hpp>
#include <project/Character.hpp>

namespace glimac {

class Camera {
public:
    virtual void moveFront(float delta) = 0; //permettant d'avancer / reculer la caméra. Lorsque delta est positif la caméra doit avancer, sinon elle doit reculer.
    virtual void rotateLeft(float degrees) = 0; // permettant de tourner latéralement autour du centre de vision.
    virtual void rotateUp(float degrees) = 0; // permettant de tourner verticalement autour du centre de vision.

	virtual glm::mat4 getViewMatrix() const = 0; // calcule la ViewMatrix de la caméra
    virtual glm::mat4 getViewMatrix(glm::mat4) const = 0; // calcule la ViewMatrix de la caméra
    virtual glm::mat4 getViewMatrix(Character*, glm::vec2 gameSize) const = 0;
};

}