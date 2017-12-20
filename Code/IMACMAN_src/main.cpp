#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "glimac/Cube.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(1280, 720, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    // Enable Program
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/normal.fs.glsl");
    program.use();

    // Get the uniforms
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Matrix declaration
    glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;

    // Projection Matrix (world) : vertical view angle, window ratio, near, far
    ProjMatrix = glm::perspective(glm::radians(70.f), windowManager.getRatio(), 0.1f, 100.f);
    // ModelView Matrix (camera)
    MVMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
    // Normal Matrix in the camera landmark
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Cube cube = Cube();

     // VBO

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
     
    // Nombre de vertex * taille des données du vertex
    glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount() * sizeof(Vertex3D), cube.getVertexBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORD = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const GLvoid*) 0);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const GLvoid*) offsetof(Vertex3D, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const GLvoid*) offsetof(Vertex3D, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // On applique les transformations
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * MVMatrix));

         // We draw
        cube.drawCube();

        // Update the display
        windowManager.swapBuffers();
    }

    // LIBERATION DES RESSOURCES
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}
