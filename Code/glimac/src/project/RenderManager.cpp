#include <iostream>

#include "project/RenderManager.hpp"

using namespace glimac;

// Constructor
RenderManager::RenderManager(SDLWindowManager* windowManager, Camera* camera, ProgramList* programList, glm::vec2 gameSize)
{
    // Window Manager
    m_windowManager = windowManager;

    // Camera
    // m_ffCamera = camera;

    // Cube
    m_cube = Cube();
    m_cubeVBO = m_cube.getVBO();
    m_cubeIBO = m_cube.getIBO();
    m_cubeVAO = m_cube.getVAO(&m_cubeIBO, &m_cubeVBO);

    // Sphere
    m_sphere = Sphere();
    m_sphereVBO = m_sphere.getVBO();
    m_sphereIBO = m_sphere.getIBO();
    m_sphereVAO = m_sphere.getVAO(&m_sphereIBO, &m_sphereVBO);

    // Matrix
    // Projection Matrix (world) : vertical view angle, window ratio, near, far
    m_ProjMatrix = glm::perspective(glm::radians(70.f), windowManager->getRatio(), 0.1f, 100.f);
    // ModelView Matrix (camera)
    m_MVMatrix = camera->getViewMatrix();
    // Normal Matrix in the camera landmark
    m_NormalMatrix = glm::transpose(glm::inverse(m_MVMatrix));

    // Textures
    m_PacmanTex = new Texture("../Code/assets/textures/EarthMap.jpg");
    m_GhostTex = new Texture("../Code/assets/textures/EarthMap.jpg");
    m_WallTex = new Texture("../Code/assets/textures/EarthMap.jpg");
    /*m_PacmanTex = new Texture("/home/daphne/PROJET_OPENGL/IMACMAN/Code/assets/textures/EarthMap.jpg");
    m_GhostTex = new Texture("/home/daphne/PROJET_OPENGL/IMACMAN/Code/assets/textures/EarthMap.jpg");
    m_WallTex = new Texture("/home/daphne/PROJET_OPENGL/IMACMAN/Code/assets/textures/EarthMap.jpg");*/

    // GLSL Program
    m_programList = programList;

    // Game Size Infos
    m_gameSize = gameSize;
    m_gameCorner = glm::vec2(-(gameSize.x / 2), -(gameSize.y / 2));
}

// Destructor
RenderManager::~RenderManager()
{
    glDeleteBuffers(1, &m_cubeVBO);
    glDeleteVertexArrays(1, &m_cubeVAO);

    glDeleteBuffers(1, &m_sphereVBO);
    glDeleteVertexArrays(1, &m_sphereVAO);

    delete(m_PacmanTex);
    delete(m_GhostTex);
    delete(m_WallTex);
}

// ---------------
// CUBE FUNCTIONS
// ---------------

// Returns pointer of cube object
Cube* RenderManager::getCubePtr()
{
    return &m_cube;
}
// Returns pointer of cube VAO
GLuint* RenderManager::getCubeVAOPtr()
{
    return &m_cubeVAO;
}

// ---------------
// SPHERE FUNCTIONS
// ---------------

// Returns pointer of sphere object
Sphere* RenderManager::getSpherePtr()
{
    return &m_sphere;
}
// Returns pointer of sphere VAO
GLuint* RenderManager::getSphereVAOPtr()
{
    return &m_sphereVAO;
}

// ---------------
// RENDERING FUNCTIONS
// ---------------

void RenderManager::bindCubeVAO()
{
    glBindVertexArray(m_cubeVAO);
}
void RenderManager::bindSphereVAO()
{
    glBindVertexArray(m_sphereVAO);
}
void RenderManager::debindVAO()
{
    glBindVertexArray(0);
}

// ---------------
// MATRIX FUNCTIONS
// ---------------

// Getters
glm::mat4 RenderManager::getProjMatrix() const
{
    return m_ProjMatrix;
}
glm::mat4 RenderManager::getMVMatrix() const
{
    return m_MVMatrix;
}
glm::mat4 RenderManager::getNormalMatrix() const
{
    return m_NormalMatrix;
}

// Update
void RenderManager::updateMVMatrix(Camera* camera)
{
    m_MVMatrix = camera->getViewMatrix();
}
void RenderManager::updateMVMatrix(Camera* camera, Character* character)
{
    m_MVMatrix = camera->getViewMatrix(character);
}

// ---------------
// TEXTURES FUNCTIONS
// ---------------

void RenderManager::loadTextures() const
{
  m_PacmanTex->loadTexture();
  m_GhostTex->loadTexture();
  m_WallTex->loadTexture();
}


// ---------------
// GLSL PROGRAM FUNCTIONS
// ---------------

// Use the correct program associated to the shader
void RenderManager::useProgram(FS shader)
{
    switch (shader)
    {
        case NORMAL :
            m_programList->normalProgram->m_Program.use();
            break;

        case TEXTURE :
            m_programList->textureProgram->m_Program.use();
            break;

        default :
            m_programList->normalProgram->m_Program.use();
            break;
    }
}

// ---------------
// GENERAL MATRIX TRANSFORMATIONS
// ---------------

// Do the correct transformations
glm::mat4 RenderManager::transformMatrix(Object* object)
{
    glm::mat4 matrix = m_MVMatrix;

    // Translate to correct Position
    matrix = glm::translate(matrix, glm::vec3(m_gameCorner.x + object->getPosX(), 0, m_gameCorner.y + object->getPosY()));
    // Rotate the object - Orientation
    matrix = glm::rotate(matrix, (float)object->getOrientation() * glm::pi<float>()/180, glm::vec3(0, 1, 0));
    // Scale the object
    matrix = glm::scale(matrix, glm::vec3(object->getWidth(), object->getHeight(), object->getWidth()));

    return matrix;
}

// Apply Transformations, Update Uniforms
void RenderManager::applyTransformations(FS shader, glm::mat4 matrix)
{
    switch (shader)
    {
        case NORMAL :
            glUniformMatrix4fv(m_programList->normalProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->normalProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->normalProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        case TEXTURE :
            glUniformMatrix4fv(m_programList->textureProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->textureProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->textureProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        default :
            break;
    }
}

// ---------------
// SPECIFIC TRANSFORMATIONS & PROGRAMS (shader)
// ---------------

// CHARACTERS

// Draw Pacman - Sphere - Shader : NORMAL
void RenderManager::drawPacman(Pacman* pacman)
{
    glm::mat4 transformationMatrix = transformMatrix(pacman);
    applyTransformations(NORMAL, transformationMatrix);
    m_sphere.drawSphere();
}

// Draw Pacman - Sphere - Shader : TEXTURE
void RenderManager::drawPacmanTex(Pacman* pacman)
{
    glUniform1i(m_programList->textureProgram->uTexture, 0);
    glm::mat4 transformationMatrix = transformMatrix(pacman);
    applyTransformations(TEXTURE, transformationMatrix);
    glBindTexture(GL_TEXTURE_2D, m_PacmanTex->getID());
    m_sphere.drawSphere();
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Draw Ghost - Sphere - Shader : NORMAL
void RenderManager::drawGhost(Ghost* ghost)
{
    glm::mat4 transformationMatrix = transformMatrix(ghost);
    applyTransformations(NORMAL, transformationMatrix);
    m_cube.drawCube();
}

// STATIC OBJECTS

// ---- 1 OBJECT ----- //

// Draw Wall - Cube - Shader : NORMAL
void RenderManager::drawWall(Wall* wall)
{
    glm::mat4 transformationMatrix = transformMatrix(wall);
    applyTransformations(NORMAL, transformationMatrix);
    m_cube.drawCube();
}

// Draw PacGomme - Sphere - Shader : NORMAL
void RenderManager::drawPacGomme(Edible* edible)
{
    glm::mat4 transformationMatrix = transformMatrix(edible);
    applyTransformations(NORMAL, transformationMatrix);
    m_sphere.drawSphere();
}

// Draw Super PacGomme - Sphere - Shader : NORMAL
void RenderManager::drawSuperPacGomme(Edible* edible)
{
    glm::mat4 transformationMatrix = transformMatrix(edible);
    applyTransformations(NORMAL, transformationMatrix);
    m_sphere.drawSphere();
}

// Draw Fruit - Sphere - Shader : NORMAL
void RenderManager::drawFruit(Edible* edible)
{
    glm::mat4 transformationMatrix = transformMatrix(edible);
    applyTransformations(NORMAL, transformationMatrix);
    m_sphere.drawSphere();
}

// ---- ALL OBJECTS ----- //

void RenderManager::drawWalls(std::vector<Wall*> wall)
{
    for (unsigned int i = 0; i < wall.size(); ++i)
    {
        drawWall(wall[i]);
    }
}

void RenderManager::drawGhosts(std::vector<Ghost*> ghost)
{
    for (unsigned int i = 0; i < ghost.size(); ++i)
    {
        drawGhost(ghost[i]);
    }
}

void RenderManager::drawPacGommes(std::vector<Edible*> edible)
{
    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        drawPacGomme(edible[i]);
    }
}

void RenderManager::drawSuperPacGommes(std::vector<Edible*> edible)
{
    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        drawSuperPacGomme(edible[i]);
    }
}

void RenderManager::drawFruits(std::vector<Edible*> edible)
{
    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        drawFruit(edible[i]);
    }
}
