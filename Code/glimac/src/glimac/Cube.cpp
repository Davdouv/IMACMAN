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
    // Left Face
    m_VertexBuffer[16].position = glm::vec3(-w, -h, t);     // Bottom Left
    m_VertexBuffer[17].position = glm::vec3(-w, -h, -t);      // Bottom Right
    m_VertexBuffer[18].position = glm::vec3(-w, h, -t);       // Up Right
    m_VertexBuffer[19].position = glm::vec3(-w, h, t);      // Up Left
    // Right Face
    m_VertexBuffer[20].position = glm::vec3(w, -h, t);     // Bottom Left
    m_VertexBuffer[21].position = glm::vec3(w, -h, -t);      // Bottom Right
    m_VertexBuffer[22].position = glm::vec3(w, h, -t);       // Up Right
    m_VertexBuffer[23].position = glm::vec3(w, h, t);      // Up Left

    // Calculate normals
    // for (unsigned int i = 0; i < getVertexCount(); i++)
    // {
    //     m_VertexBuffer[i].normal = normalize(m_VertexBuffer[i].position);
    // }

    // Front Face
    m_VertexBuffer[0].normal = glm::vec3(0, 0, 1);     // Bottom Left
    m_VertexBuffer[1].normal = glm::vec3(0, 0, 1);      // Bottom Right
    m_VertexBuffer[2].normal = glm::vec3(0, 0, 1);       // Up Right
    m_VertexBuffer[3].normal = glm::vec3(0, 0, 1);      // Up Left
    // Back Face
    m_VertexBuffer[4].normal = glm::vec3(0, 0, -1);     // Bottom Left
    m_VertexBuffer[5].normal = glm::vec3(0, 0, -1);      // Bottom Right
    m_VertexBuffer[6].normal = glm::vec3(0, 0, -1);       // Up Right
    m_VertexBuffer[7].normal = glm::vec3(0, 0, -1);      // Up Left
    // Upper Face
    m_VertexBuffer[8].normal = glm::vec3(0, 1, 0);     // Bottom Left
    m_VertexBuffer[9].normal = glm::vec3(0, 1, 0);      // Bottom Right
    m_VertexBuffer[10].normal = glm::vec3(0, 1, 0);       // Up Right
    m_VertexBuffer[11].normal = glm::vec3(0, 1, 0);      // Up Left
    // Bottom Face
    m_VertexBuffer[12].normal = glm::vec3(0, -1, 0);     // Bottom Left
    m_VertexBuffer[13].normal = glm::vec3(0, -1, 0);      // Bottom Right
    m_VertexBuffer[14].normal = glm::vec3(0, -1, 0);       // Up Right
    m_VertexBuffer[15].normal = glm::vec3(0, -1, 0);      // Up Left
    // Left Face
    m_VertexBuffer[16].normal = glm::vec3(-1, 0, 0);     // Bottom Left
    m_VertexBuffer[17].normal = glm::vec3(-1, 0, 0);      // Bottom Right
    m_VertexBuffer[18].normal = glm::vec3(-1, 0, 0);       // Up Right
    m_VertexBuffer[19].normal = glm::vec3(-1, 0, 0);      // Up Left
        // Bottom Face
    m_VertexBuffer[20].normal = glm::vec3(1, 0, 0);     // Bottom Left
    m_VertexBuffer[21].normal = glm::vec3(1, 0, 0);      // Bottom Right
    m_VertexBuffer[22].normal = glm::vec3(1, 0, 0);       // Up Right
    m_VertexBuffer[23].normal = glm::vec3(1, 0, 0);      // Up Left


    // Texture Coordinates
    for (int i = 0; i < (int)this->getVertexCount(); i = i + 4)
    {
        m_VertexBuffer[i].texCoords = glm::vec2(0,1);
        m_VertexBuffer[i+1].texCoords = glm::vec2(1,1);
        m_VertexBuffer[i+2].texCoords = glm::vec2(1,0);
        m_VertexBuffer[i+3].texCoords = glm::vec2(0,0);
    }

    // Front face
    m_IndexBuffer[0] = 0;
    m_IndexBuffer[1] = 1;
    m_IndexBuffer[2] = 2;

    m_IndexBuffer[3] = 0;
    m_IndexBuffer[4] = 2;
    m_IndexBuffer[5] = 3;

    // Left face
    m_IndexBuffer[6] = 16;
    m_IndexBuffer[7] = 17;
    m_IndexBuffer[8] = 18;

    m_IndexBuffer[9] = 16;
    m_IndexBuffer[10] = 18;
    m_IndexBuffer[11] = 19;

    // Back Face
    m_IndexBuffer[12] = 4;
    m_IndexBuffer[13] = 7;
    m_IndexBuffer[14] = 5;

    m_IndexBuffer[15] = 5;
    m_IndexBuffer[16] = 6;
    m_IndexBuffer[17] = 7;

    // Right Face
    m_IndexBuffer[18] = 20;
    m_IndexBuffer[19] = 21;
    m_IndexBuffer[20] = 22;

    m_IndexBuffer[21] = 20;
    m_IndexBuffer[22] = 22;
    m_IndexBuffer[23] = 23;

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

    // Upper Face
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
