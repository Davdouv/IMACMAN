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

    // Skybox
    m_skybox = new StaticObject('K', 15.f, 20.f, 300.f, 300.f);

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
    m_ProjMatrix = glm::perspective(glm::radians(70.f), windowManager->getRatio(), 0.1f, 200.f);
    // ModelView Matrix (camera)
    m_MVMatrix = camera->getViewMatrix();
    // Normal Matrix in the camera landmark
    m_NormalMatrix = glm::transpose(glm::inverse(m_MVMatrix));

    // Textures
    m_PacmanTexture = new Texture("../Code/assets/textures/pacman.jpg");
    m_GhostTexture = new Texture("../Code/assets/textures/ghost.jpg");
    m_WallTexture = new Texture("../Code/assets/textures/wall.jpg");
    m_GumTexture = new Texture("../Code/assets/textures/gum.jpg");
    m_SuperGumTexture = new Texture("../Code/assets/textures/superpacgum.jpg");
    m_FruitTexture = new Texture("../Code/assets/textures/fruit.jpg");
    m_SkyboxTexture = new CubeMap("../Code/assets/textures/darkness.jpg",
                                  "../Code/assets/textures/darkness.jpg",
                                  "../Code/assets/textures/darkness.jpg",
                                  "../Code/assets/textures/darkness.jpg",
                                  "../Code/assets/textures/darkness.jpg",
                                  "../Code/assets/textures/darkness.jpg"
                                );

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

    delete(m_PacmanTexture);
    delete(m_GhostTexture);
    delete(m_WallTexture);
    delete(m_GumTexture);
    delete(m_SuperGumTexture);
    delete(m_FruitTexture);
    delete(m_SkyboxTexture);
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
    //m_MVMatrix = camera->getViewMatrix(transformMatrix(character));
    m_MVMatrix = camera->getViewMatrix(character, m_gameCorner);
}

// ---------------
// TEXTURES FUNCTIONS
// ---------------

void RenderManager::loadTextures() const
{
  m_PacmanTexture->loadTexture();
  m_GhostTexture->loadTexture();
  m_WallTexture->loadTexture();
  m_GumTexture->loadTexture();
  m_SuperGumTexture->loadTexture();
  m_FruitTexture->loadTexture();
  m_SkyboxTexture->loadCubeMap();

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

        case CUBEMAP :
            m_programList->cubemapProgram->m_Program.use();
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
            glUniform1i(m_programList->textureProgram->uTexture, 0);

            glUniformMatrix4fv(m_programList->textureProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->textureProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->textureProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        case CUBEMAP :
            glUniformMatrix4fv(m_programList->cubemapProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->cubemapProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->cubemapProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        default :
            break;
    }
}

// Enable texture if we use shader texture
void RenderManager::enableTexture(FS shader, Texture* texture)
{
    if (shader == TEXTURE)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getID());
    }
}

// Disable texture if we use shader texture
void RenderManager::disableTexture(FS shader)
{
    if (shader == TEXTURE)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
    }
}

// ---------------
// SPECIFIC TRANSFORMATIONS & PROGRAMS (shader)
// ---------------

// ---- CHARACTERS ----- //

// PACMAN
void RenderManager::drawPacman(Pacman* pacman, FS shader)
{
    useProgram(shader);

    glm::mat4 transformationMatrix = transformMatrix(pacman);
    applyTransformations(shader, transformationMatrix);

    enableTexture(shader, m_PacmanTexture);

    m_sphere.drawSphere();

    disableTexture(shader);
}

// GHOSTS
void RenderManager::drawGhosts(std::vector<Ghost*> ghost, FS shader)
{
    for (unsigned int i = 0; i < ghost.size(); ++i)
    {
        if (ghost[i]->getSuper())
        {
            useProgram(NORMAL);

            glm::mat4 transformationMatrix = transformMatrix(ghost[i]);
            applyTransformations(NORMAL, transformationMatrix);
            m_cube.drawCube();
        }
        else
        {
            useProgram(shader);

            enableTexture(shader, m_GhostTexture);

            glm::mat4 transformationMatrix = transformMatrix(ghost[i]);
            applyTransformations(shader, transformationMatrix);
            m_cube.drawCube();

            disableTexture(shader);
        }
    }
}

// ---- ALL OBJECTS ----- //


// WALLS
void RenderManager::drawWalls(std::vector<Wall*> wall, FS shader)
{
    useProgram(shader);
    enableTexture(shader, m_WallTexture);

    for (unsigned int i = 0; i < wall.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(wall[i]);
        applyTransformations(shader, transformationMatrix);
        m_cube.drawCube();
    }

    disableTexture(shader);
}

// GOMMES
void RenderManager::drawPacGommes(std::vector<Edible*> edible, FS shader)
{
    useProgram(shader);
    enableTexture(shader, m_GumTexture);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(edible[i]);
        applyTransformations(shader, transformationMatrix);
        m_sphere.drawSphere();
    }

    disableTexture(shader);
}

// SUPER GOMMES
void RenderManager::drawSuperPacGommes(std::vector<Edible*> edible, FS shader)
{
    useProgram(shader);
    enableTexture(shader, m_SuperGumTexture);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(edible[i]);
        applyTransformations(shader, transformationMatrix);
        m_sphere.drawSphere();
    }

    disableTexture(shader);
}

// FRUITS
void RenderManager::drawFruits(std::vector<Edible*> edible, FS shader)
{
    useProgram(shader);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        if (edible[i]->getAvailability())
        {
            enableTexture(shader, m_FruitTexture);
            glm::mat4 transformationMatrix = transformMatrix(edible[i]);
            applyTransformations(shader, transformationMatrix);
            m_sphere.drawSphere();
            disableTexture(shader);
        }
    }
}


// ---- SKYBOX ----- //
void RenderManager::initSkybox()
{
  useProgram(CUBEMAP);
  glm::mat4 transformationMatrix = transformMatrix(m_skybox);
  applyTransformations(CUBEMAP, transformationMatrix);
}

void RenderManager::drawSkybox()
{
  glDepthMask(GL_FALSE);
  useProgram(CUBEMAP);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture->getID());
  m_cube.drawCube();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDepthMask(GL_TRUE);
}
