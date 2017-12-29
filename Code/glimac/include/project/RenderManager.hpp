#pragma once

#include "glimac/Cube.hpp"
#include "glimac/Sphere.hpp"
#include <glimac/SDLWindowManager.hpp>
#include "glimac/Camera.hpp"

#include "project/Object.hpp"
#include "project/Character.hpp"
#include "project/Pacman.hpp"
#include "project/Ghost.hpp"
#include "project/Wall.hpp"
#include "project/Edible.hpp"
#include "project/Texture.hpp"

#include "project/GLSLProgram.hpp"

using namespace glimac;

class RenderManager {

private:
    // Window Manager
    SDLWindowManager* m_windowManager;

    // Cameras
    // TrackballCamera* m_tbCamera;    // TPS Cam
    // FreeflyCamera* m_ffCamera;      // FPS Cam
    // bool m_fps;                     // false = TPS | true = FPS

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

    // GLSL Programs
    ProgramList* m_programList;

    // Game Size Infos
    glm::vec2 m_gameSize;
    glm::vec2 m_gameCorner;

public:
    // Constructor - SDLWindowManager for Ratio - Camera for viewMatrix - FilePath for Shaders
    RenderManager(SDLWindowManager* windowManager, Camera* camera, ProgramList* programList, glm::vec2 gameSize);
    // Destructor
    ~RenderManager();

    // Cube getters
    Cube* getCubePtr();
    GLuint* getCubeVAOPtr();

    // Sphere getters
    Sphere* getSpherePtr();
    GLuint* getSphereVAOPtr();

    // Rendering functions
    void bindCubeVAO();
    void bindSphereVAO();
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

    // GLSL Programs functions
    void useProgram(FS shader);

    // Matrix Transformations
    glm::mat4 transformMatrix(Object* object);
    void applyTransformations(FS shader, glm::mat4 matrix);

    // Specific Transformations & Programs
    void drawPacman(Pacman* pacman, FS shader = NORMAL);
    void drawPacmanNormal(Pacman* pacman);
    void drawPacmanTexture(Pacman* pacman);
    void drawWalls(std::vector<Wall*>, FS shader = NORMAL);
    void drawGhosts(std::vector<Ghost*>, FS shader = NORMAL);
    void drawPacGommes(std::vector<Edible*>, FS shader = NORMAL);
    void drawSuperPacGommes(std::vector<Edible*>, FS shader = NORMAL);
    void drawFruits(std::vector<Edible*>, FS shader = NORMAL);

private:
    void drawWallNormal(Wall* wall);
    void drawWallTexture(Wall* wall);
    void drawGhostNormal(Ghost* ghost);
    void drawGhostTexture(Ghost* ghost);
    void drawPacGommeNormal(Edible* edible);
    void drawPacGommeTexture(Edible* edible);
    void drawSuperPacGommeNormal(Edible* edible);
    void drawSuperPacGommeTexture(Edible* edible);
    void drawFruitNormal(Edible* edible);
    void drawFruitTexture(Edible* edible);

    // Switch camera
    // void switchCamera();
};
