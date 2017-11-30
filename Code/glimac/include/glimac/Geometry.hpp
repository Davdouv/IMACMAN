#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Image.hpp"
#include "FilePath.hpp"
#include "BBox.hpp"

namespace glimac {

class Geometry {
public:
    struct Vertex {
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec2 m_TexCoords;
    };

    struct Mesh {
        std::string m_sName;
        unsigned int m_nIndexOffset; // Offset in the index buffer
        unsigned int m_nIndexCount; // Number of indices
        int m_nMaterialIndex; // -1 if no material assigned

        Mesh(std::string name, unsigned int indexOffset, unsigned int indexCount, int materialIndex):
            m_sName(move(name)), m_nIndexOffset(indexOffset), m_nIndexCount(indexCount), m_nMaterialIndex(materialIndex) {
        }
    };

    struct Material {
        glm::vec3 m_Ka;
        glm::vec3 m_Kd;
        glm::vec3 m_Ks;
        glm::vec3 m_Tr;
        glm::vec3 m_Le;
        float m_Shininess;
        float m_RefractionIndex;
        float m_Dissolve;
        const Image* m_pKaMap;
        const Image* m_pKdMap;
        const Image* m_pKsMap;
        const Image* m_pNormalMap;
    };

private:
    std::vector<Vertex> m_VertexBuffer;
    std::vector<unsigned int> m_IndexBuffer;
    std::vector<Mesh> m_MeshBuffer;
    std::vector<Material> m_Materials;
    BBox3f m_BBox;

    void generateNormals(unsigned int meshIndex);

public:
    const Vertex* getVertexBuffer() const {
        return m_VertexBuffer.data();
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

    const Mesh* getMeshBuffer() const {
        return m_MeshBuffer.data();
    }

    size_t getMeshCount() const {
        return m_MeshBuffer.size();
    }

    bool loadOBJ(const FilePath& filepath, const FilePath& mtlBasePath, bool loadTextures = true);

    const BBox3f& getBoundingBox() const {
        return m_BBox;
    }
};

}
