#pragma once

#include <vector>
#include "common.hpp"

namespace glimac {
    
// Représente un cone ouvert discrétisé dont la base est centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Cone {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat height, GLfloat radius, GLsizei discLat, GLsizei discHeight);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Cone(GLfloat height, GLfloat radius, GLsizei discLat, GLsizei discHeight):
        m_nVertexCount(0) {
        build(height, radius, discLat, discHeight); // Construction (voir le .cpp)
    }

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
    
}
