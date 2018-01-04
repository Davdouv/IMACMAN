#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Sphere {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat radius, GLsizei discLat, GLsizei discLong);

public:
    
    Sphere() { build (1,32,16); }

    Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong) {
        build(radius, discLat, discLong); // Construction (voir le .cpp)
    }

    const Vertex3D* getVertexBuffer() const {
        return &m_VertexBuffer[0];
    }

    size_t getVertexCount() const {
        return m_VertexBuffer.size();
    }

    const unsigned int* getIndexBuffer() const {
        return m_IndexBuffer.data();
    }

    size_t getIndexCount() const {
        return m_IndexBuffer.size();
    }

    // Draw the Sphere
    void drawSphere() const
    {
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, 0);
    }

    // Draw half of the sphere, true for upper side, false for bottom side
    void drawHalfSphere(bool side) const
    {
        glDrawElements(GL_TRIANGLES, this->getIndexCount()/2, GL_UNSIGNED_INT, (GLvoid*)(side*(getIndexCount()/2)*sizeof(GLuint)));
    }

    GLuint getVBO();
    GLuint getIBO();
    GLuint getVAO(GLuint* ibo, GLuint* vbo);

private:
    std::vector<Vertex3D> m_VertexBuffer;
    std::vector<uint32_t> m_IndexBuffer;
};
    
}