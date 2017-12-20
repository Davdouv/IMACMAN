#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

class Cube 
{
    // Allocates and constructs the data (implementation in the .cpp)
    void build(GLsizei width, GLsizei height, GLsizei thickness);

public:
    // Constructor: allocates the data array and builds the vertex attributes
    Cube() { build (1,1,1); }
    Cube(GLsizei width, GLsizei height, GLsizei thickness)
    {
        build(width, height, thickness); // Construction (see the .cpp)
    }

    // Returns the pointer to the data
    const Vertex3D* getVertexBuffer() const {
        return &m_VertexBuffer[0];
    }

    // Returns the number of vertices
    size_t getVertexCount() const {
        return 8;
    }

    // Returns the address of the IBO
    const unsigned int* getIndexBuffer() const {
        return &m_IndexBuffer[0];
    }

    // Returns the number of Index (of Triangles)
    size_t getIndexCount() const {
        return 32;
    }

    // Draw the Cube
    void drawCube() {
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, 0);
    }

private:
    Vertex3D m_VertexBuffer[8];
    unsigned int m_IndexBuffer[32];
};

}