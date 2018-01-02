#pragma once

#include "glimac/Plane.hpp"
#include "glimac/Cube.hpp"
#include "glimac/Sphere.hpp"
#include <glimac/SDLWindowManager.hpp>
#include "glimac/Camera.hpp"

#include "project/Map.hpp"
#include "project/Controller.hpp"
#include "project/Texture.hpp"
#include "project/CubeMap.hpp"

#include "project/GLSLProgram.hpp"

using namespace glimac;

class RenderManager {

private:
    // Window Manager
    SDLWindowManager* m_windowManager;

    // Plane
    Plane m_plane;
    GLuint m_planeVBO;
    GLuint m_planeIBO;
    GLuint m_planeVAO;

    // Cube
    Cube m_cube;
    GLuint m_cubeVBO;
    GLuint m_cubeIBO;
    GLuint m_cubeVAO;

    // Sphere
    Sphere m_sphere;
    GLuint m_sphereVBO;
    GLuint m_sphereIBO;
    GLuint m_sphereVAO;

    // Matrix
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_MVMatrix;
    glm::mat4 m_NormalMatrix;

    // Textures
    Texture* m_PacmanTexture;
    Texture* m_GhostTexture;
    Texture* m_WallTexture;
    Texture* m_GumTexture;
    Texture* m_SuperGumTexture;
    Texture* m_FruitTexture;
    CubeMap* m_SkyboxTexture;
    Texture* m_FloorTexture;
    Texture* m_RenderTargetTexture;

    // GLSL Programs
    ProgramList* m_programList;

    // Game Size Infos
    glm::vec2 m_gameSize;
    glm::vec2 m_gameCorner;

    // Skybox
    StaticObject* m_skybox;

    // Floor
    StaticObject* m_floor;

    // Mini-Map
    StaticObject* m_miniMap;

public:
    // Constructor - SDLWindowManager for Ratio - Camera for viewMatrix - FilePath for Shaders
    RenderManager(SDLWindowManager* windowManager, Camera* camera, ProgramList* programList, glm::vec2 gameSize);
    // Destructor
    ~RenderManager();

    // Skybox
    void initSkybox();
    Cube* getCubeSkyboxPtr();
    GLuint* getCubeSkyboxVAOPtr();

    // Cube getters
    Cube* getCubePtr();
    GLuint* getCubeVAOPtr();

    // Sphere getters
    Sphere* getSpherePtr();
    GLuint* getSphereVAOPtr();

    // Plane getters
    Plane* getPlanePtr();
    GLuint* getPlaneVAOPtr();

    // Rendering functions
    void bindCubeVAO();
    void bindSphereVAO();
    void bindPlaneVAO();
    void debindVAO();

    // Matrix functions
    // Getters
    glm::mat4 getProjMatrix() const;
    glm::mat4 getMVMatrix() const;
    glm::mat4 getNormalMatrix() const;
    // Update
    void updateMVMatrix(Camera* camera);
    void updateMVMatrix(Camera* camera, Character* character);

    // Texture functions
    void loadTextures() const;
    void enableTexture(FS shader, Texture* texture);
    void disableTexture(FS shader);

    // GLSL Programs functions
    void useProgram(FS shader);

    // Matrix Transformations
    glm::mat4 transformMatrix(Object* object);
    void applyTransformations(FS shader, glm::mat4 matrix);

    // Material Transformations (for light shader)
    void materialTransformations(FS shader, float Kd, float Ks, float shininess);

    // Specific Transformations & Programs
    void drawPacman(Pacman* pacman, FS shader = NORMAL);
    void drawWalls(std::vector<Wall*>, FS shader = NORMAL);
    void drawGhosts(std::vector<Ghost*>, FS shader = NORMAL);
    void drawPacGommes(std::vector<Edible*>, FS shader = NORMAL);
    void drawSuperPacGommes(std::vector<Edible*>, FS shader = NORMAL);
    void drawFruits(std::vector<Edible*>, FS shader = NORMAL);
    void drawSkybox();
    void drawFloor(FS shader);

    // Mini-map
    void drawMiniMap(FS shader);    // NOT WORKING ACTUALLY

    // Global
    void drawMap(Map* map, Controller* controller);

};
