#pragma once

#include <vector>

#include "glimac/common.hpp"

namespace glimac {

class Plane
{
    // Allocates and constructs the data (implementation in the .cpp)
    void build();

public:
    // Constructor: allocates the data array and builds the vertex attributes
    Plane() { build (); }

    // Returns the pointer to the data
    const Vertex3D* getVertexBuffer() const {
        return &m_VertexBuffer[0];
    }

    // Returns the number of vertices
    size_t getVertexCount() const {
        return 4;
    }

    // Returns the address of the IBO
    const unsigned int* getIndexBuffer() const {
        return &m_IndexBuffer[0];
    }

    // Returns the number of Index (of Triangles)
    size_t getIndexCount() const {
        return 6;
    }

    // Draw the Cube
    void drawPlane() const
    {
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, 0);
    }

    GLuint getVBO();
    GLuint getIBO();
    GLuint getVAO(GLuint* ibo, GLuint* vbo);

private:
    Vertex3D m_VertexBuffer[4];
    uint32_t m_IndexBuffer[6];
};

}
