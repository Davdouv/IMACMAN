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

float norm(glm::vec3 V);

glm::vec3 normalize(glm::vec3 V);

GLuint VBO(GLsizeiptr size, const GLvoid* data);
GLuint IBO(GLsizeiptr size, const GLvoid* data);
GLuint VAO(GLuint* ibo, GLuint* vbo);

}
