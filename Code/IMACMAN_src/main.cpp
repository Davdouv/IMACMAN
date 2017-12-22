#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "glimac/Cube.hpp"
#include "glimac/Sphere.hpp"

#include "glimac/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "project/Controller.hpp"

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
    GLint uMVPMatrix;       // ModelViewProjection Matrix        
    GLint uMVMatrix;        // ModelView Matrix             Camera Space
    GLint uNormalMatrix;    // Normal Matrix                For Light
    GLint uTexture;

    uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Matrix declaration
    //TrackballCamera camera = TrackballCamera(30,0,0.0f,1.57f);    // CAMERA VUE 2D
    TrackballCamera camera = TrackballCamera(30,0,0.0f,1.0f);

    glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;

    // Projection Matrix (world) : vertical view angle, window ratio, near, far
    ProjMatrix = glm::perspective(glm::radians(70.f), windowManager.getRatio(), 0.1f, 100.f);
    // ModelView Matrix (camera)
    MVMatrix = camera.getViewMatrix();
    // Normal Matrix in the camera landmark
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Cube cube = Cube();

    GLuint cubeVBO = cube.getVBO();
    GLuint cubeIBO = cube.getIBO();
    GLuint cubeVAO = cube.getVAO(&cubeIBO, &cubeVBO);

    Sphere sphere = Sphere();

    GLuint sphereVBO = sphere.getVBO();
    GLuint sphereIBO = sphere.getIBO();
    GLuint sphereVAO = sphere.getVAO(&sphereIBO, &sphereVBO);

    Controller controller = Controller(&windowManager);
    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
            
            controller.updateController();
        }

        camera.cameraController(&controller);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // On récupére la ViewMatrix à chaque tour de boucle
        glm::mat4 globalMVMatrix = camera.getViewMatrix();

        // CUBES
        glBindVertexArray(cubeVAO);
        glm::mat4 cubeMatrix;

        for (int i = -15; i < 15; i++)
        {            
            // Transformations
            cubeMatrix = glm::translate(globalMVMatrix, glm::vec3(i,0,-15));
            // On applique les transformations
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            // We draw
            cube.drawCube();
        }
        for (int i = 0; i < 30; i++)
        {            
            cubeMatrix = glm::translate(globalMVMatrix, glm::vec3(-15,0,-14+i));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            cube.drawCube();
        }
        for (int i = 0; i < 30; i++)
        {            
            cubeMatrix = glm::translate(globalMVMatrix, glm::vec3(14,0,-14+i));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            cube.drawCube();
        }
        for (int i = -15; i < 15; i++)
        {            
            cubeMatrix = glm::translate(globalMVMatrix, glm::vec3(i,0,16));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            cube.drawCube();
        }
        
        glBindVertexArray(0);


        // // SPHERES
        // glBindVertexArray(sphereVAO);
        // // On récupére la ViewMatrix à chaque tour de boucle
        // glm::mat4 sphereMVMatrix = camera.getViewMatrix();
        // // On applique les transformations
        // glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(sphereMVMatrix));
        // glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(sphereMVMatrix))));
        // glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * sphereMVMatrix));
        //  // We draw
        // sphere.drawSphere();
        // glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    // LIBERATION DES RESSOURCES
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteVertexArrays(1, &sphereVAO);

    return EXIT_SUCCESS;
}