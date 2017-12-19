#include "glimac/Geometry.hpp"
#include "tiny_obj_loader.h"
#include <iostream>
#include <algorithm>

namespace glimac {

void Geometry::generateNormals(unsigned int meshIndex) {
    auto indexOffset = m_MeshBuffer[meshIndex].m_nIndexOffset;
    for (auto j = 0u; j < m_MeshBuffer[meshIndex].m_nIndexCount; j += 3) {
        auto i1 = m_IndexBuffer[indexOffset + j];
        auto i2 = m_IndexBuffer[indexOffset + j + 1];
        auto i3 = m_IndexBuffer[indexOffset + j + 2];

        auto n = glm::cross(glm::normalize(m_VertexBuffer[i2].m_Position - m_VertexBuffer[i1].m_Position),
                            glm::normalize(m_VertexBuffer[i3].m_Position - m_VertexBuffer[i1].m_Position));

        m_VertexBuffer[i1].m_Normal = n;
        m_VertexBuffer[i2].m_Normal = n;
        m_VertexBuffer[i3].m_Normal = n;
    }
}

bool Geometry::loadOBJ(const FilePath& filepath, const FilePath& mtlBasePath, bool loadTextures) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::clog << "Load OBJ " << filepath << std::endl;
    std::string objErr = tinyobj::LoadObj(shapes, materials,
        filepath.c_str(), mtlBasePath.c_str());

    std::clog << "done." << std::endl;

    if (!objErr.empty()) {
        std::cerr << objErr << std::endl;
        return false;
    }

    std::clog << "Load materials" << std::endl;
    m_Materials.reserve(m_Materials.size() + materials.size());
    for(auto& material: materials) {
        m_Materials.emplace_back();
        auto& m = m_Materials.back();

        m.m_Ka = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
        m.m_Kd = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        m.m_Ks = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
        m.m_Tr = glm::vec3(material.transmittance[0], material.transmittance[1], material.transmittance[2]);
        m.m_Le = glm::vec3(material.emission[0], material.emission[1], material.emission[2]);
        m.m_Shininess = material.shininess;
        m.m_RefractionIndex = material.ior;
        m.m_Dissolve = material.dissolve;

        if(loadTextures) {
            if(!material.ambient_texname.empty()) {
                //std::replace(material.ambient_texname.begin(), material.ambient_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.ambient_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKaMap = ImageManager::loadImage(texturePath);
            }

            if(!material.diffuse_texname.empty()) {
                //std::replace(material.diffuse_texname.begin(), material.diffuse_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.diffuse_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKdMap = ImageManager::loadImage(texturePath);
            }

            if(!material.specular_texname.empty()) {
                //std::replace(material.specular_texname.begin(), material.specular_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.specular_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pKsMap = ImageManager::loadImage(texturePath);
            }

            if(!material.normal_texname.empty()) {
                //std::replace(material.normal_texname.begin(), material.normal_texname.end(), '\\', '/');
                FilePath texturePath = mtlBasePath + material.normal_texname;
                std::clog << "load " << texturePath << std::endl;
                m.m_pNormalMap = ImageManager::loadImage(texturePath);
            }
        }
    }
    std::clog << "done." << std::endl;

    auto globalVertexOffset = m_VertexBuffer.size();
    auto globalIndexOffset = m_IndexBuffer.size();

    auto nbVertex = 0u;
    auto nbIndex = 0u;
    for (const auto& shape: shapes) {
        nbVertex += shape.mesh.positions.size();
        nbIndex += shape.mesh.indices.size();
    }

    std::clog << "Number of meshes: " << shapes.size() << std::endl;
    std::clog << "Number of vertices: " << nbVertex << std::endl;
    std::clog << "Number of triangles: " << (nbIndex) / 3 << std::endl;

    m_BBox = BBox3f(glm::vec3(shapes[0].mesh.positions[0], shapes[0].mesh.positions[1], shapes[0].mesh.positions[2]));

    m_VertexBuffer.resize(m_VertexBuffer.size() + nbVertex);
    m_IndexBuffer.resize(m_IndexBuffer.size() + nbIndex);

    auto pVertex = (Vertex*) m_VertexBuffer.data() + globalVertexOffset;
    auto pIndex = (unsigned int*) m_IndexBuffer.data() + globalIndexOffset;

    m_MeshBuffer.reserve(m_MeshBuffer.size() + shapes.size());

    auto vertexOffset = globalVertexOffset;
    auto indexOffset = globalIndexOffset;
    for (size_t i = 0; i < shapes.size(); i++) {
        auto pVertexTmp = pVertex;
        for (auto j = 0u; j < shapes[i].mesh.positions.size(); j += 3) {
            pVertexTmp->m_Position.x = shapes[i].mesh.positions[j];
            pVertexTmp->m_Position.y = shapes[i].mesh.positions[j + 1];
            pVertexTmp->m_Position.z = shapes[i].mesh.positions[j + 2];

            m_BBox.grow(pVertexTmp->m_Position);

            ++pVertexTmp;
        }
        pVertexTmp = pVertex;
        if(shapes[i].mesh.normals.size()) {
            for (auto j = 0u; j < shapes[i].mesh.normals.size(); j += 3) {
                pVertexTmp->m_Normal.x = shapes[i].mesh.normals[j];
                pVertexTmp->m_Normal.y = shapes[i].mesh.normals[j + 1];
                pVertexTmp->m_Normal.z = shapes[i].mesh.normals[j + 2];
                ++pVertexTmp;
            }
        } else {
            // should create normals from triangles...
        }
        pVertexTmp = pVertex;
        if(shapes[i].mesh.texcoords.size()) {
            for (auto j = 0u; j < shapes[i].mesh.texcoords.size(); j += 2) {
                pVertexTmp->m_TexCoords.x = shapes[i].mesh.texcoords[j];
                pVertexTmp->m_TexCoords.y = shapes[i].mesh.texcoords[j + 1];
                ++pVertexTmp;
            }
        } else {
            for (auto j = 0u; j < shapes[i].mesh.texcoords.size(); j += 2) {
                pVertexTmp->m_TexCoords.x =  0.f;
                pVertexTmp->m_TexCoords.x =  0.f;
                ++pVertexTmp;
            }
        }

        for (auto j = 0u; j < shapes[i].mesh.indices.size(); ++j) {
            pIndex[j] = vertexOffset + shapes[i].mesh.indices[j];
        }

        int materialIndex = -1;
        if(!shapes[i].mesh.material_ids.empty()) {
            materialIndex = shapes[i].mesh.material_ids[0];
        }

        m_MeshBuffer.emplace_back(shapes[i].name, indexOffset, shapes[i].mesh.indices.size(), materialIndex);

        if(shapes[i].mesh.normals.size() == 0u) {
            generateNormals(m_MeshBuffer.size() - 1);
        }

        pVertex += shapes[i].mesh.positions.size();
        vertexOffset += shapes[i].mesh.positions.size();
        pIndex += shapes[i].mesh.indices.size();
        indexOffset += shapes[i].mesh.indices.size();
    }

    return true;
}

}
