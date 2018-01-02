#include <cmath>
#include <iostream>
#include "glimac/Plane.hpp"

namespace glimac {

void Plane::build() {

    // Build all vertices
    // Front face
    m_VertexBuffer[0].position = glm::vec3(-0.5, -0.5, 0.0);     // Bottom Left
    m_VertexBuffer[1].position = glm::vec3(0.5, -0.5, 0.0);      // Bottom Right
    m_VertexBuffer[2].position = glm::vec3(0.5, 0.5, 0.0);       // Up Right
    m_VertexBuffer[3].position = glm::vec3(-0.5, 0.5, 0.0);      // Up Left

    // Calculate normals
    for (unsigned int i = 0; i < 4; i++)
    {
        m_VertexBuffer[i].normal = normalize(m_VertexBuffer[i].position);
    }

    // Texture Coordinates
    m_VertexBuffer[0].texCoords = glm::vec2(0,1);
    m_VertexBuffer[1].texCoords = glm::vec2(1,1);
    m_VertexBuffer[2].texCoords = glm::vec2(1,0);
    m_VertexBuffer[3].texCoords = glm::vec2(0,0);

    // Index Buffer
    m_IndexBuffer[0] = 0;
    m_IndexBuffer[1] = 1;
    m_IndexBuffer[2] = 2;

    m_IndexBuffer[3] = 0;
    m_IndexBuffer[4] = 2;
    m_IndexBuffer[5] = 3;
}

GLuint Plane::getVBO()
{
    return VBO(this->getVertexCount(), this->getVertexBuffer());
}

GLuint Plane::getIBO()
{
    return IBO(this->getIndexCount(), this->getIndexBuffer());
}

GLuint Plane::getVAO(GLuint* ibo, GLuint* vbo)
{
    return VAO(ibo, vbo);
}



}
