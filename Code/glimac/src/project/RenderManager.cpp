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

    // Plane
    m_plane = Plane();
    m_planeVBO = m_plane.getVBO();
    m_planeIBO = m_plane.getIBO();
    m_planeVAO = m_plane.getVAO(&m_planeIBO, &m_planeVBO);

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

    // Model3D
    buildModel(&m_model[0], &m_modelVBO[0], &m_modelIBO[0], &m_modelVAO[0], "ghost/snapchat.obj", "ghost/snapchat.mtl");
    //buildModel(&m_model[1], &m_modelVBO[1], &m_modelIBO[1], &m_modelVAO[1], "banana/banana.obj", "");

    // Matrices
    // Projection Matrix (world) : vertical view angle, window ratio, near, far
    m_ProjMatrix = glm::perspective(glm::radians(70.f), windowManager->getRatio(), 0.1f, 300.f);
    // ModelView Matrix (camera)
    m_MVMatrix = camera->getViewMatrix();
    // Normal Matrix in the camera landmark
    m_NormalMatrix = glm::transpose(glm::inverse(m_MVMatrix));

    // Textures
    m_PacmanTexture = new Texture("../Code/assets/textures/pacman2.png");
    m_GhostTexture = new Texture("../Code/assets/textures/ghost.jpg");
    m_WallTexture = new Texture("../Code/assets/textures/dungeon.png");
    m_GumTexture = new Texture("../Code/assets/textures/gum.jpg");
    m_SuperGumTexture = new Texture("../Code/assets/textures/superpacgum.jpg");
    m_FruitTexture = new Texture("../Code/assets/textures/fruit.jpg");
      m_SkyboxTexture = new CubeMap("../Code/assets/textures/VolcanoCM/negz.jpg",
                                    "../Code/assets/textures/VolcanoCM/posz.jpg",
                                    "../Code/assets/textures/VolcanoCM/posy.jpg",
                                    "../Code/assets/textures/VolcanoCM/negy.jpg",
                                    "../Code/assets/textures/VolcanoCM/negx.jpg",
                                  "../Code/assets/textures/VolcanoCM/posx.jpg"
                                );
    m_FloorTexture = new Texture("../Code/assets/textures/lava2.jpg");

    m_RenderTargetTexture = new Texture();

    // Text
    Text::initText();

    // GLSL Program
    m_programList = programList;

    // Game Size Infos
    m_gameSize = gameSize;
    m_gameCorner = glm::vec2(-(gameSize.x / 2), -(gameSize.y / 2));

    // Skybox
    m_skybox = new StaticObject('K', gameSize.y/2, gameSize.x/2, 300.f, 300.f);

    // Floor
    m_floor = new StaticObject('F', gameSize.y/2, gameSize.x/2, gameSize.y, gameSize.x);

    // Mini Map
    m_miniMap = new StaticObject('F', gameSize.y/2, gameSize.x/2, gameSize.y, gameSize.x);

    // State
    m_state = GameManager::PacmanState::NORMAL;

    // Score Panel
    m_SP_titleSurface = NULL;
		m_SP_pointsSurface = NULL;
	  m_SP_timeSurface = NULL;
	  m_SP_pointsScoreSurface = NULL;
    m_SP_timeScoreSurface = NULL;
	  m_SP_Surface = NULL;

    m_stop = 0.f;
}

// Destructor
RenderManager::~RenderManager()
{
    glDeleteBuffers(1, &m_cubeVBO);
    glDeleteVertexArrays(1, &m_cubeVAO);

    glDeleteBuffers(1, &m_sphereVBO);
    glDeleteVertexArrays(1, &m_sphereVAO);

    glDeleteBuffers(1, &m_planeVBO);
    glDeleteVertexArrays(1, &m_planeVAO);

    // Textures
    delete(m_PacmanTexture);
    delete(m_GhostTexture);
    delete(m_WallTexture);
    delete(m_GumTexture);
    delete(m_SuperGumTexture);
    delete(m_FruitTexture);
    delete(m_SkyboxTexture);
    delete(m_FloorTexture);
    delete(m_RenderTargetTexture);

    // Static Objects
    delete(m_skybox);
    delete(m_floor);
    delete(m_miniMap);

    Text::clean();
}

// ---------------
// TEXT FUNCTIONS
// ---------------

void RenderManager::loadFont()
{
  m_font = Text::loadFont("../Code/assets/fonts/DejaVuSans.ttf");
}

SDL_Surface* RenderManager::createTextTexture(GLuint* textImg, std::string text, SDL_Color color)
{
  m_SP_Surface = SDL_DisplayFormatAlpha(TTF_RenderUTF8_Solid( m_font, text.data(), color ));
  int colors = m_SP_Surface->format->BytesPerPixel;
  SDL_Rect area;
  area.x = 0; area.y = 0; area.w = m_SP_Surface->w; area.h = m_SP_Surface->h;
  GLenum texture_format = GL_RGBA;

  glDisable(GL_TEXTURE);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
  GLuint img;

  glGenTextures(1, &img);
  glBindTexture(GL_TEXTURE_2D, img);
  glTexImage2D( GL_TEXTURE_2D,
                0,
                colors,
                m_SP_Surface->w,
                m_SP_Surface->h,
                0,
                texture_format,
                GL_UNSIGNED_BYTE,
                m_SP_Surface->pixels
              );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
  *textImg = img;

  return m_SP_Surface;
}

void RenderManager::drawText(SDL_Surface* textSurface, GLuint textImg, float size, float x, float y)
{
  useProgram(TEXTURE);
  bindPlaneVAO();
  glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -5.0f));
  float ratio = floatDivision(textSurface->w, textSurface->h);
  matrix = glm::scale(matrix, glm::vec3(size * ratio, size, 1.f));
  applyTransformations(TEXTURE, matrix);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textImg);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  m_plane.drawPlane();
  disableTexture(TEXTURE, false);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  debindVAO();
}

void RenderManager::drawText(SDL_Surface* textSurface, GLuint textImg, float size, glm::mat4 matrix)
{
  useProgram(TEXTURE);
  bindPlaneVAO();
  float ratio = floatDivision(textSurface->w, textSurface->h);
  matrix = glm::scale(matrix, glm::vec3(size * ratio, size, 1.f));
  applyTransformations(TEXTURE, matrix);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textImg);
  glEnable(GL_TEXTURE_2D);
  m_plane.drawPlane();
  disableTexture(TEXTURE, false);
  glDisable(GL_TEXTURE_2D);
  debindVAO();
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
// PLANE FUNCTIONS
// ---------------

// Returns pointer of plane object
Plane* RenderManager::getPlanePtr()
{
    return &m_plane;
}
// Returns pointer of plane VAO
GLuint* RenderManager::getPlaneVAOPtr()
{
    return &m_planeVAO;
}

// ---------------
// SCORE PANEL FUNCTIONS
// ---------------

void RenderManager::createScorePanel(int points)
{
  if (m_SP_titleSurface == NULL)
  {
    m_SP_titleSurface = createTextTexture(&m_SP_titleImg, "SCORE", {255,255,255});
    m_SP_pointsSurface = createTextTexture(&m_SP_pointsImg, "Points:", {255,255,255});
    m_SP_timeSurface = createTextTexture(&m_SP_timeImg, "Time:", {255,255,255});
    m_SP_pointsScoreSurface = createTextTexture(&m_SP_pointsScoreImg, std::to_string(points), {255,255,255});
    m_SP_timeScoreSurface = createTextTexture(&m_SP_timeScoreImg, m_time, {255,255,255});
  }
}

void RenderManager::drawScorePanel(int points)
{
  useProgram(TEXTURE);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glm::mat4 original_matrix = m_MVMatrix;
  original_matrix = glm::translate(original_matrix, glm::vec3(-3.5f, -0.6f, 2.0f));  // Some values set to adjust the plane
  original_matrix = glm::rotate(original_matrix, (float)-90 * glm::pi<float>()/180, glm::vec3(1, 0, 0));
  original_matrix = glm::scale(original_matrix, glm::vec3(-1.f, 1.f, 1.f));

  glm::mat4 matrix = glm::translate(original_matrix, glm::vec3(0.f, 10.0f, 0.f));
  if (m_SP_titleSurface == NULL)
    m_SP_titleSurface = createTextTexture(&m_SP_titleImg, "SCORE", {255,255,255});
  drawText(m_SP_titleSurface, m_SP_titleImg, 5.f, matrix);

  matrix = glm::translate(original_matrix, glm::vec3(0.f, 6.0f, -0.1f));
  if (m_SP_pointsSurface == NULL)
    m_SP_pointsSurface = createTextTexture(&m_SP_pointsImg, "Points:", {255,255,255});
  drawText(m_SP_pointsSurface, m_SP_pointsImg, 2.f, matrix);

  matrix = glm::translate(original_matrix, glm::vec3(0.f, 3.5f, -0.1f));
  if (m_SP_pointsScoreSurface == NULL)
    m_SP_pointsScoreSurface = createTextTexture(&m_SP_pointsScoreImg, std::to_string(points), {255,255,255});
  drawText(m_SP_pointsScoreSurface, m_SP_pointsScoreImg, 3.5f, matrix);

  matrix = glm::translate(original_matrix, glm::vec3(0.f, 0.f, -0.0f));
  if (m_SP_timeSurface == NULL)
    m_SP_timeSurface = createTextTexture(&m_SP_timeImg, "Time:", {255,255,255});
  drawText(m_SP_timeSurface, m_SP_timeImg, 2.f, matrix);

  matrix = glm::translate(original_matrix, glm::vec3(0.f, -2.5f, -0.1f));
  if (m_SP_timeScoreSurface == NULL)
    m_SP_timeScoreSurface = createTextTexture(&m_SP_timeScoreImg, m_time, {255,255,255});
  drawText(m_SP_timeScoreSurface, m_SP_timeScoreImg, 3.5f, matrix);
  glDisable(GL_BLEND);
}

// ---------------
// 3D MODEL FUNCTIONS
// ---------------

// Build 3D Model
void RenderManager::buildModel(Geometry* objModel, GLuint* VBO, GLuint* IBO, GLuint* VAO, std::string objFile, std::string mtlFile)
{
    objFile = "../Code/assets/models/"+objFile;
    mtlFile = "../Code/assets/models/"+mtlFile;

    objModel->loadOBJ(objFile, mtlFile, true);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    // Nombre de vertex * taille des données du vertex
    glBufferData(GL_ARRAY_BUFFER, objModel->getVertexCount() * sizeof(Geometry::Vertex), objModel->getVertexBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // => Creation du IBO
    glGenBuffers(1, IBO);

    // => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);


    // => On remplit l'IBO avec les indices:
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objModel->getIndexCount() * sizeof(uint32_t), objModel->getIndexBuffer(), GL_STATIC_DRAW);

    // => Comme d'habitude on debind avant de passer à autre chose
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // VAO
    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORD = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*) 0);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*) offsetof(Geometry::Vertex, m_Normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*) offsetof(Geometry::Vertex, m_TexCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
void RenderManager::bindPlaneVAO()
{
    glBindVertexArray(m_planeVAO);
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

void RenderManager::updateMVMatrix(Camera* camera, Character* character, bool lost)
{
    m_MVMatrix = camera->getViewMatrix(character, m_gameCorner);
    if (lost)
    {
        if (m_stop < 1)
        {
            m_stop += 0.0005;
        }
        m_MVMatrix = glm::rotate(m_MVMatrix,m_stop* glm::pi<float>(), glm::vec3(0, 0, 1));
    }
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
    m_FloorTexture->loadTexture();
    m_RenderTargetTexture->loadTexture();
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

        case DIRECTIONNAL_LIGHT :
            m_programList->directionnalLightProgram->m_Program.use();
            break;

        case POINT_LIGHT :
            m_programList->pointLightProgram->m_Program.use();
            break;

        case BLACK_AND_WHITE :
            m_programList->bwProgram->m_Program.use();
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
    glm::mat4 lightMatrix;
    glm::vec4 lightVector;
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

            glUniform1f(m_programList->textureProgram->uTime, 0);

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

        case DIRECTIONNAL_LIGHT :
            glUniform1i(m_programList->directionnalLightProgram->uTexture, 0);
            // White Color to keep the correct color
            glUniform3f(m_programList->directionnalLightProgram->uColor, 1.0,1.0,1.0);

            lightMatrix = glm::rotate(m_MVMatrix, 180.f, glm::vec3(1,1,1));
            lightVector = glm::normalize(glm::vec4(1,1,1,0)*lightMatrix);
            glUniform3f(m_programList->directionnalLightProgram->uLightDir_vs, lightVector.x, lightVector.y, lightVector.z);

            if (m_state == GameManager::PacmanState::NORMAL)
                glUniform3f(m_programList->directionnalLightProgram->uLightIntensity, 2.0,2.0,2.0);
            else
                glUniform3f(m_programList->directionnalLightProgram->uLightIntensity, 2.0,1.0,1.0);

            glUniformMatrix4fv(m_programList->directionnalLightProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->directionnalLightProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->directionnalLightProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        case POINT_LIGHT :
            glUniform1i(m_programList->pointLightProgram->uTexture, 0);

            lightMatrix = glm::scale(matrix, glm::vec3(1000.f, 1000.f, 1000.f));
            lightVector = glm::normalize(glm::vec4(1,1,1,0)*matrix);
            glUniform3f(m_programList->pointLightProgram->uLightPos_vs, lightVector.x, lightVector.y, lightVector.z);
            glUniform3f(m_programList->pointLightProgram->uLightIntensity, 2.0,2.0,2.0);

            glUniformMatrix4fv(m_programList->pointLightProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->pointLightProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->pointLightProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;

        case BLACK_AND_WHITE :
            glUniform1i(m_programList->bwProgram->uTexture, 0);

            lightMatrix = glm::rotate(m_MVMatrix, 180.f, glm::vec3(1,1,1));
            lightVector = glm::normalize(glm::vec4(1,1,1,0)*lightMatrix);
            glUniform3f(m_programList->bwProgram->uLightDir_vs, lightVector.x, lightVector.y, lightVector.z);
            glUniform3f(m_programList->bwProgram->uLightIntensity, 2.0,2.0,2.0);

            glUniformMatrix4fv(m_programList->bwProgram->uMVPMatrix, 1, GL_FALSE,
            glm::value_ptr(m_ProjMatrix * matrix));

            glUniformMatrix4fv(m_programList->bwProgram->uMVMatrix, 1, GL_FALSE,
            glm::value_ptr(matrix));

            glUniformMatrix4fv(m_programList->bwProgram->uNormalMatrix, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(matrix))));
            break;


        default :
            break;
    }
}

// Material Transformations for light
void RenderManager::materialTransformations(FS shader, float Kd, float Ks, float shininess)
{
    if ((shader == DIRECTIONNAL_LIGHT) || (shader == POINT_LIGHT) || (shader == BLACK_AND_WHITE))
    {
        glUniform3f(m_programList->directionnalLightProgram->uKd, Kd, Kd, Kd);
        glUniform3f(m_programList->directionnalLightProgram->uKs,  Ks, Ks, Ks);
        glUniform1f(m_programList->directionnalLightProgram->uShininess, shininess);
    }
}

// Enable texture if we use shader texture
void RenderManager::enableTexture(FS shader, Texture* texture, bool alpha)
{
    if (shader != NORMAL)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getID());
        if (alpha)
        {
            // For alpha
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
}

// Disable texture if we use shader texture
void RenderManager::disableTexture(FS shader, bool alpha)
{
    if (shader != NORMAL)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        if (alpha)
        {
            glDisable(GL_BLEND);
        }
    }
}

// ---------------
// SPECIFIC TRANSFORMATIONS & PROGRAMS (shader)
// ---------------

// ---- CHARACTERS ----- //

// PACMAN
void RenderManager::drawPacman(Pacman* pacman, FS shader)
{
    //useProgram(shader);

    materialTransformations(shader, 0.7, 0.3, 100);
    enableTexture(shader, m_PacmanTexture, false);

    glm::mat4 transformationMatrix = transformMatrix(pacman);
    transformationMatrix = glm::rotate(transformationMatrix, glm::pi<float>(), glm::vec3(0, 1, 0));
    transformationMatrix = glm::rotate(transformationMatrix, glm::pi<float>()/8, glm::vec3(1, 0, 0));

    float angle = fabs(glm::cos(glm::pi<float>()/180 * 0.2f * SDL_GetTicks()));

    // Top sphere
    glm::mat4 rotateUpMat = glm::rotate(transformationMatrix, 60.f * glm::pi<float>()/180 * angle, glm::vec3(1, 0, 0));
    applyTransformations(shader, rotateUpMat);
    m_sphere.drawHalfSphere(1);

    // Bottom sphere
    glm::mat4 rotateDownMat = glm::rotate(transformationMatrix, -20.f * glm::pi<float>()/180 * angle, glm::vec3(1, 0, 0));
    applyTransformations(shader, rotateDownMat);
    m_sphere.drawHalfSphere(0);

    //m_sphere.drawSphere();

    disableTexture(shader, false);
}

// GHOSTS
void RenderManager::drawGhosts(std::vector<Ghost*> ghost, FS shader)
{
    for (unsigned int i = 0; i < ghost.size(); ++i)
    {
        //useProgram(shader);

        enableTexture(shader, m_GumTexture/*m_GhostTexture*/, true);

        glm::mat4 transformationMatrix = transformMatrix(ghost[i]);
        transformationMatrix = glm::rotate(transformationMatrix, -1.57f, glm::vec3(0,1,0));
        applyTransformations(shader, transformationMatrix);
        materialTransformations(shader, 1.0, 0.0, 100);
        if (ghost[i]->getSuper())
        {
            glUniform3f(m_programList->directionnalLightProgram->uColor, 0.0,0.0,1.0);
        }
        else
        {
            glm::vec3 color = ghost[i]->getColor();
            glUniform3f(m_programList->directionnalLightProgram->uColor, color.r,color.g,color.b);
        }
        //m_cube.drawCube();
        glDrawElements(GL_TRIANGLES, m_model[0].getIndexCount(), GL_UNSIGNED_INT, 0);

        disableTexture(shader, true);

    }
}

// ---- ALL OBJECTS ----- //

// WALLS
void RenderManager::drawWalls(std::vector<Wall*> wall, FS shader)
{
    //useProgram(shader);
    enableTexture(shader, m_WallTexture, false);

    for (unsigned int i = 0; i < wall.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(wall[i]);
        applyTransformations(shader, transformationMatrix);
        materialTransformations(shader, 0.9, 0.1, 20);
        m_cube.drawCube();
        //glDrawElements(GL_TRIANGLES, m_rock.getIndexCount(), GL_UNSIGNED_INT, 0);
    }

    disableTexture(shader, false);
}

// GOMMES
void RenderManager::drawPacGommes(std::vector<Edible*> edible, FS shader)
{
    //useProgram(shader);
    enableTexture(shader, m_GumTexture,false);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(edible[i]);
        applyTransformations(shader, transformationMatrix);
        materialTransformations(shader, 0.9, 0.1, 50);
        m_sphere.drawSphere();
    }

    disableTexture(shader, false);
}

// SUPER GOMMES
void RenderManager::drawSuperPacGommes(std::vector<Edible*> edible, FS shader)
{
    //useProgram(shader);
    enableTexture(shader, m_SuperGumTexture, false);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        glm::mat4 transformationMatrix = transformMatrix(edible[i]);
        applyTransformations(shader, transformationMatrix);
        materialTransformations(shader, 0.8, 0.2, 50);
        m_sphere.drawSphere();
    }

    disableTexture(shader, false);
}

// FRUITS
void RenderManager::drawFruits(std::vector<Edible*> edible, FS shader)
{
    //useProgram(shader);

    for (unsigned int i = 0; i < edible.size(); ++i)
    {
        if (edible[i]->getAvailability())
        {
            enableTexture(shader, m_FruitTexture, false);
            glm::mat4 transformationMatrix = transformMatrix(edible[i]);
            applyTransformations(shader, transformationMatrix);
            materialTransformations(shader, 0.9, 0.3, 50);
            //glDrawElements(GL_TRIANGLES, m_model[1].getIndexCount(), GL_UNSIGNED_INT, 0);
            m_sphere.drawSphere();
            disableTexture(shader, false);
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
  //useProgram(CUBEMAP);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture->getID());
  m_cube.drawCube();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDepthMask(GL_TRUE);
}

// ---- FLOOR ---- //
void RenderManager::drawFloor(FS shader)
{
    //useProgram(shader);

    // Matrix Transformations
    glm::mat4 matrix = m_MVMatrix;
    matrix = glm::translate(matrix, glm::vec3(-3.5f, -0.5f, 2.5f));  // Some values set to adjust the plane
    matrix = glm::rotate(matrix, (float)-90 * glm::pi<float>()/180, glm::vec3(1, 0, 0));
    matrix = glm::scale(matrix, glm::vec3(m_floor->getWidth(), m_floor->getHeight(), 1.f));

    applyTransformations(shader, matrix);

    glUniform1f(m_programList->textureProgram->uTime, 0.00001f*SDL_GetTicks());

    enableTexture(shader, m_FloorTexture, false);
    m_plane.drawPlane();
    disableTexture(shader, false);
}

// ---- MINI MAP ---- //
void RenderManager::drawMiniMap(FS shader)
{
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // Matrix Transformations
    glm::mat4 matrix = m_MVMatrix;
    matrix = glm::translate(matrix, glm::vec3(-3.5f, -0.5f, 2.5f));  // Some values set to adjust the plane
    //matrix = glm::rotate(matrix, (float)-90 * glm::pi<float>()/180, glm::vec3(1, 0, 0));
    matrix = glm::scale(matrix, glm::vec3(m_floor->getWidth(), m_floor->getHeight(), 1.f));

    applyTransformations(shader, matrix);

    enableTexture(shader, m_RenderTargetTexture, false);
    m_plane.drawPlane();
    disableTexture(shader, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the
}

// ---- GLOBAL ---- //

// Draw the map (Characters & Objects)
void RenderManager::drawMap(Map* map, Controller* controller)
{
    //FS shader;
    // --- PLANE OBJECTS --- //
    bindPlaneVAO();

    useProgram(TEXTURE);
    drawFloor(TEXTURE);
    //drawMiniMap(TEXTURE);

    debindVAO();

    //--- CUBES OBJECTS --- //
    bindCubeVAO();

    useProgram(CUBEMAP);
    drawSkybox();

    // if (m_state == GameManager::PacmanState::NORMAL)
    //     shader = DIRECTIONNAL_LIGHT;
    // else
    //     shader = BLACK_AND_WHITE;

    useProgram(DIRECTIONNAL_LIGHT);

    drawWalls(map->getWalls(), DIRECTIONNAL_LIGHT);

    debindVAO();

    // --- SPHERES OBJECTS --- //
    // Bind Sphere VAO
    bindSphereVAO();

    // Draw Pacman only in TPS
    if(!controller->isFPSactive())
        drawPacman(map->getPacman(), DIRECTIONNAL_LIGHT);
    drawPacGommes(map->getPacGommes(), DIRECTIONNAL_LIGHT);
    drawSuperPacGommes(map->getSuperPacGommes(), DIRECTIONNAL_LIGHT);
    //De-bind Sphere VAO
    //debindVAO();

    //glBindVertexArray(m_modelVAO[1]);
    drawFruits(map->getFruits(), DIRECTIONNAL_LIGHT);
    debindVAO();

    //useProgram(DIRECTIONNAL_LIGHT);
    glBindVertexArray(m_modelVAO[0]);
    drawGhosts(map->getGhosts(), DIRECTIONNAL_LIGHT);
    debindVAO();
}

// Update State

void RenderManager::updateState(GameManager::PacmanState state)
{
    m_state = state;
}

// ---- MENU ---- //

void RenderManager::drawMenu(Menu* menu)
{
    bool alpha = false;
    FS shader = TEXTURE;
    bindPlaneVAO();

    useProgram(shader);

    glm::mat4 originMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    glm::mat4 matrix = originMatrix;

    if (menu->isMainMenu())
    {
        // Background
        matrix = glm::scale(matrix, glm::vec3(12.8f, 7.2f, 1.f));
        applyTransformations(shader, matrix);
        enableTexture(shader, menu->getTextures()[4], false);
        m_plane.drawPlane();
        disableTexture(shader, false);

        alpha = true;
    }

    float zDistance = 4.5f;

    // Play | Restart
    matrix = originMatrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.2f, zDistance));
    matrix = glm::scale(matrix, glm::vec3(0.2f, 0.10f, 1.f));
    applyTransformations(shader, matrix);
    enableTexture(shader, menu->getTextures()[Menu::PLAY], alpha);
    m_plane.drawPlane();
    disableTexture(shader, alpha);

    // Continue | Save
    matrix = originMatrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, zDistance));
    matrix = glm::scale(matrix, glm::vec3(0.2f, 0.10f, 1.f));
    applyTransformations(shader, matrix);
    enableTexture(shader, menu->getTextures()[Menu::CONTINUE], alpha);
    m_plane.drawPlane();
    disableTexture(shader, alpha);

    // Exit
    matrix = originMatrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, -0.2f, zDistance));
    matrix = glm::scale(matrix, glm::vec3(0.2f, 0.10f, 1.f));
    applyTransformations(shader, matrix);
    enableTexture(shader, menu->getTextures()[Menu::EXIT], alpha);
    m_plane.drawPlane();
    disableTexture(shader, alpha);

    // Select
    matrix = originMatrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, -0.2*(menu->getButton())+0.2f, zDistance));
    matrix = glm::scale(matrix, glm::vec3(0.22f, 0.12f, 1.f));
    applyTransformations(shader, matrix);
    enableTexture(shader, menu->getTextures()[3], false);
    m_plane.drawPlane();
    disableTexture(shader, false);

    debindVAO();
}

// ---- UI ---- //

std::string RenderManager::getTimeString(Uint32 final_time)
{
  std::string current_time;
  if (final_time > 1000000000)
    current_time = "-:-:-";
  else {
    int m_time  = final_time/(60*1000);
    int s_time  = ((final_time%(60*1000))/1000);
    int ms_time = (((final_time%(60*10000))%1000) / 100);
    //current_time = std::to_string(m_time) + ":" + std::to_string(s_time) + ":" + std::to_string(ms_time);
    current_time = std::to_string(m_time) + ":" + std::to_string(s_time);
  }
  m_time = current_time;
  return current_time;
}

void RenderManager::drawUI(UI* ui, Uint32 start_game_time, Uint32 pause_time, Uint32 total_time)
{
    FS shader = TEXTURE;
    bindPlaneVAO();

    useProgram(shader);

    glm::mat4 originMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

    // Life
    for (unsigned int i = 0; i < ui->getPlayer()->getLife(); ++i)
    {
        glm::mat4 matrix = originMatrix;
        matrix = glm::translate(matrix, glm::vec3(5.5f-(0.5*i), 3.0f, 0.2f));
        matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 1.f));
        applyTransformations(shader, matrix);
        enableTexture(shader, ui->getTextures()[0], true);
        m_plane.drawPlane();
        disableTexture(shader, true);
    }
    debindVAO();

    // Points
    m_scoreSurface = createTextTexture(&m_scoreImg, std::to_string( ui->getPlayer()->getPoints()), {255,255,255});
    drawText(m_scoreSurface, m_scoreImg, 0.3f,
             5.5f, 2.5f);

    // Time
    Uint32 final_time = SDL_GetTicks() - start_game_time - pause_time + total_time;
    if ((((final_time%(60*10000))%1000) / 100) == 0)
    {
      m_timeSurface = createTextTexture(&m_timeImg, getTimeString(final_time), {255,255,255});
    }
    if (m_timeSurface != NULL)
      drawText(m_timeSurface, m_timeImg, 0.25f,
             5.3f, 2.0f);
}
