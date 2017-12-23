#pragma once

#include "glimac/Cube.hpp"
#include "glimac/Sphere.hpp"
#include <glimac/SDLWindowManager.hpp>
#include "glimac/Camera.hpp"

using namespace glimac;

class RenderManager {

private:
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

public:
    // Constructor
    RenderManager(SDLWindowManager* windowManager, Camera* camera);
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
    glm::mat4* getProjMatrix();
    glm::mat4* getMVMatrix();
    glm::mat4* getNormalMatrix();
    // Update
    void updateMVMatrix(Camera* camera);
};