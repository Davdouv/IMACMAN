#pragma once

#include <GL/glew.h>
#include "glm.hpp"
#include <math.h>

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Vertex3D {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

float norm(glm::vec3 V)
{
	return sqrt((V.x * V.x) + (V.y * V.y) + (V.z * V.z));
}

glm::vec3 normalize(glm::vec3 V)
{
	float normf = norm(V);
	glm::vec3 vector;
	vector.x = V.x / normf;
	vector.y = V.y / normf;
    vector.z = V.z / normf;

	return vector;
}

}
