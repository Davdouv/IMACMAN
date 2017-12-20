#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cube.hpp"

namespace glimac {

void Cube::build(GLsizei width = 1.0, GLsizei height = 1.0, GLsizei thickness = 1.0) {

    float w,h,t;
    w = width/2;
    h = height/2;
    t = thickness/2;
    
    // Build all vertices
    for (unsigned int i = 0; i <= 1; i++)
    {
        m_VertexBuffer[0+4*i].position = glm::vec3(-w, -h, t*(-1+2*i));     // Bottom Left
        m_VertexBuffer[1+4*i].position = glm::vec3(w, -h, t*(-1+2*i));      // Bottom Right
        m_VertexBuffer[2+4*i].position = glm::vec3(w, h, t*(-1+2*i));       // Up Right
        m_VertexBuffer[3+4*i].position = glm::vec3(-w, h, t*(-1+2*i));      // UP Left
    }

    // Calculate normals
    for (unsigned int i = 0; i < 8; i++)
    {
        m_VertexBuffer[i].normal = normalize(m_VertexBuffer[i].position);
    }

    // Front face
    m_IndexBuffer[0] = 0;
    m_IndexBuffer[1] = 1;
    m_IndexBuffer[2] = 2;

    m_IndexBuffer[3] = 0;
    m_IndexBuffer[4] = 2;
    m_IndexBuffer[5] = 3;

    // Left face
    m_IndexBuffer[6] = 0;
    m_IndexBuffer[7] = 3;
    m_IndexBuffer[8] = 4;

    m_IndexBuffer[6] = 3;
    m_IndexBuffer[7] = 4;
    m_IndexBuffer[8] = 7;

    // Back Face
    m_IndexBuffer[9] = 4;
    m_IndexBuffer[10] = 7;
    m_IndexBuffer[11] = 5;

    m_IndexBuffer[12] = 5;
    m_IndexBuffer[13] = 6;
    m_IndexBuffer[14] = 7;

    // Right Face
    m_IndexBuffer[15] = 5;
    m_IndexBuffer[16] = 6;
    m_IndexBuffer[17] = 2;

    m_IndexBuffer[18] = 5;
    m_IndexBuffer[19] = 2;
    m_IndexBuffer[20] = 1;

    // Upper Face
    m_IndexBuffer[21] = 2;
    m_IndexBuffer[22] = 6;
    m_IndexBuffer[23] = 7;

    m_IndexBuffer[24] = 2;
    m_IndexBuffer[25] = 7;
    m_IndexBuffer[26] = 3;

    // Bottom Face
    m_IndexBuffer[27] = 0;
    m_IndexBuffer[28] = 1;
    m_IndexBuffer[29] = 4;

    m_IndexBuffer[30] = 1;
    m_IndexBuffer[31] = 4;
    m_IndexBuffer[32] = 5;
}

}
