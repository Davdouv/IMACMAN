#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Cone.hpp"

namespace glimac {

void Cone::build(GLfloat height, GLfloat r, GLsizei discLat, GLsizei discHeight) {
    // Equation paramétrique en (r, phi, h) du cone
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= h <= height
    //
    // x(r, phi, h) = r (height - h) sin(phi) / height
    // y(r, phi, h) = h
    // z(r, phi, h) = r (height - h) cos(phi) / height
    //
    // Discrétisation:
    // dPhi = 2PI / discLat, dH = height / discHeight
    //
    // x(r, i, j) = r * (height - j * dH) * sin(i * dPhi) / height
    // y(r, i, j) = j * dH
    // z(r, i, j) = r * (height - j * dH) * cos(i * dPhi) / height

    GLfloat rcpLat = 1.f / discLat, rcpH = 1.f / discHeight;
    GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dH = height * rcpH;
    
    std::vector<ShapeVertex> data;
    
    // Construit l'ensemble des vertex
    for(GLsizei j = 0; j <= discHeight; ++j) {
        for(GLsizei i = 0; i < discLat; ++i) {
            ShapeVertex vertex;
            
            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = j * rcpH;
            
            vertex.position.x = r * (height - j * dH) * sin(i * dPhi) / height;
            vertex.position.y = j * dH;
            vertex.position.z = r * (height - j * dH) * cos(i * dPhi) / height;
            
            /* avec cette formule la normale est mal définie au sommet (= (0, 0, 0))
            vertex.normal.x = vertex.position.x;
            vertex.normal.y = r * r * (1 - vertex.position.y / height) / height;
            vertex.normal.z = vertex.position.z;
            vertex.normal = glm::normalize(vertex.normal);
            */
            
            vertex.normal.x = sin(i * dPhi);
            vertex.normal.y = r / height;
            vertex.normal.z = cos(i * dPhi);
            vertex.normal = glm::normalize(vertex.normal);
            
            data.push_back(vertex);
        }
    }

    m_nVertexCount = discLat * discHeight * 6;
    
    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for(GLsizei j = 0; j < discHeight; ++j) {
        GLsizei offset = j * discLat;
        for(GLsizei i = 0; i < discLat; ++i) {
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + i + discLat]);
        }
    }
    
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs
}

}


