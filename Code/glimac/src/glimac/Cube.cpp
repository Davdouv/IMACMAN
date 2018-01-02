#include <cmath>
#include <iostream>
#include "glimac/Cube.hpp"

namespace glimac {

void Cube::build(GLfloat width = 1.0, GLfloat height = 1.0, GLfloat thickness = 1.0) {

    float w,h,t;
    w = width/2;        // 0.5
    h = height/2;       // 0.5
    t = thickness/2;    // 0.5

    // Build all vertices
    // Front face
    m_VertexBuffer[0].position = glm::vec3(-w, -h, t);     // Bottom Left
    m_VertexBuffer[1].position = glm::vec3(w, -h, t);      // Bottom Right
    m_VertexBuffer[2].position = glm::vec3(w, h, t);       // Up Right
    m_VertexBuffer[3].position = glm::vec3(-w, h, t);      // Up Left
    // Back Face
    m_VertexBuffer[4].position = glm::vec3(-w, -h, -t);     // Bottom Left
    m_VertexBuffer[5].position = glm::vec3(w, -h, -t);      // Bottom Right
    m_VertexBuffer[6].position = glm::vec3(w, h, -t);       // Up Right
    m_VertexBuffer[7].position = glm::vec3(-w, h, -t);      // Up Left
    // Upper Face
    m_VertexBuffer[8].position = glm::vec3(-w, h, t);     // Bottom Left
    m_VertexBuffer[9].position = glm::vec3(w, h, t);      // Bottom Right
    m_VertexBuffer[10].position = glm::vec3(w, h, -t);       // Up Right
    m_VertexBuffer[11].position = glm::vec3(-w, h, -t);      // Up Left
    // Bottom Face
    m_VertexBuffer[12].position = glm::vec3(-w, -h, t);     // Bottom Left
    m_VertexBuffer[13].position = glm::vec3(w, -h, t);      // Bottom Right
    m_VertexBuffer[14].position = glm::vec3(w, -h, -t);       // Up Right
    m_VertexBuffer[15].position = glm::vec3(-w, h, -t);      // Up Left

    // Calculate normals
    for (unsigned int i = 0; i < getVertexCount(); i++)
    {
        m_VertexBuffer[i].normal = normalize(m_VertexBuffer[i].position);
    }

    // Texture Coordinates
    // Front face
    m_VertexBuffer[0].texCoords = glm::vec2(0,1);
    m_VertexBuffer[1].texCoords = glm::vec2(1,1);
    m_VertexBuffer[2].texCoords = glm::vec2(1,0);
    m_VertexBuffer[3].texCoords = glm::vec2(0,0);
    // Back face
    m_VertexBuffer[4].texCoords = glm::vec2(1,1);
    m_VertexBuffer[5].texCoords = glm::vec2(0,1);
    m_VertexBuffer[6].texCoords = glm::vec2(0,0);
    m_VertexBuffer[7].texCoords = glm::vec2(1,0);
    // Upper face
    m_VertexBuffer[8].texCoords = glm::vec2(0,1);
    m_VertexBuffer[9].texCoords = glm::vec2(1,1);
    m_VertexBuffer[10].texCoords = glm::vec2(1,0);
    m_VertexBuffer[11].texCoords = glm::vec2(0,0);
    // // Bottom face
    m_VertexBuffer[12].texCoords = glm::vec2(0,1);
    m_VertexBuffer[13].texCoords = glm::vec2(1,1);
    m_VertexBuffer[14].texCoords = glm::vec2(1,0);
    m_VertexBuffer[15].texCoords = glm::vec2(0,0);


    // Front face
    m_IndexBuffer[0] = 0;
    m_IndexBuffer[1] = 1;
    m_IndexBuffer[2] = 2;

    m_IndexBuffer[3] = 0;
    m_IndexBuffer[4] = 2;
    m_IndexBuffer[5] = 3;

    // // Left face
    m_IndexBuffer[6] = 0;
    m_IndexBuffer[7] = 3;
    m_IndexBuffer[8] = 4;

    m_IndexBuffer[9] = 3;
    m_IndexBuffer[10] = 4;
    m_IndexBuffer[11] = 7;

    // // Back Face
    m_IndexBuffer[12] = 4;
    m_IndexBuffer[13] = 7;
    m_IndexBuffer[14] = 5;

    m_IndexBuffer[15] = 5;
    m_IndexBuffer[16] = 6;
    m_IndexBuffer[17] = 7;

    // Right Face
    m_IndexBuffer[18] = 2;
    m_IndexBuffer[19] = 5;
    m_IndexBuffer[20] = 6;

    m_IndexBuffer[21] = 1;
    m_IndexBuffer[22] = 2;
    m_IndexBuffer[23] = 5;

    // Upper Face
    // m_IndexBuffer[24] = 2;
    // m_IndexBuffer[25] = 6;
    // m_IndexBuffer[26] = 7;

    // m_IndexBuffer[27] = 2;
    // m_IndexBuffer[28] = 7;
    // m_IndexBuffer[29] = 3;

    // Bottom Face
    m_IndexBuffer[30] = 0;
    m_IndexBuffer[31] = 1;
    m_IndexBuffer[32] = 4;

    m_IndexBuffer[33] = 1;
    m_IndexBuffer[34] = 4;
    m_IndexBuffer[35] = 5;

    // // Upper Face
    m_IndexBuffer[24] = 8;
    m_IndexBuffer[25] = 9;
    m_IndexBuffer[26] = 10;

    m_IndexBuffer[27] = 8;
    m_IndexBuffer[28] = 10;
    m_IndexBuffer[29] = 11;

    // Bottom Face
    m_IndexBuffer[30] = 12;
    m_IndexBuffer[31] = 13;
    m_IndexBuffer[32] = 14;

    m_IndexBuffer[33] = 12;
    m_IndexBuffer[34] = 14;
    m_IndexBuffer[35] = 15;
}

GLuint Cube::getVBO()
{
    return VBO(this->getVertexCount(), this->getVertexBuffer());
}

GLuint Cube::getIBO()
{
    return IBO(this->getIndexCount(), this->getIndexBuffer());
}

GLuint Cube::getVAO(GLuint* ibo, GLuint* vbo)
{
    return VAO(ibo, vbo);
}



}
