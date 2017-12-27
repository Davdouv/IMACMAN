#pragma once

#include "glimac/Cube.hpp"
#include "glimac/Sphere.hpp"
#include <glimac/SDLWindowManager.hpp>
#include "glimac/Camera.hpp"

#include "project/Object.hpp"
#include "project/Pacman.hpp"
#include "project/Ghost.hpp"
#include "project/Wall.hpp"
#include "project/Edible.hpp"

#include "project/GLSLProgram.hpp"

using namespace glimac;

class RenderManager {

private:
    // Window Manager
    SDLWindowManager* m_windowManager;

    // Cameras
    // TrackballCamera* m_tbCamera;    // TPS Cam
    // FreeflyCamera* m_ffCamera;      // FPS Cam
    // bool m_fps;                     // false = TPS | true = FPS

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

    // GLSL Programs functions
    void useProgram(FS shader);

    // Matrix Transformations
    glm::mat4 transformMatrix(Object* object);
    void applyTransformations(FS shader, glm::mat4 matrix);

    // Specific Transformations & Programs
    void drawPacman(Pacman* pacman);
    void drawGhost(Ghost* ghost);
    void drawWalls(std::vector<Wall*>);
    void drawPacGommes(std::vector<Edible*>);
    void drawSuperPacGommes(std::vector<Edible*>);
    void drawFruits(std::vector<Edible*>);

private:
    void drawWall(Wall* wall);
    void drawPacGomme(Edible* edible);
    void drawSuperPacGomme(Edible* edible);
    void drawFruit(Edible* edible);

    // Switch camera
    // void switchCamera();
};