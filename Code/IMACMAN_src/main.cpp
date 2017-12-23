#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "project/RenderManager.hpp"

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

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    RenderManager renderManager = RenderManager(&windowManager, &camera);

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
        renderManager.updateMVMatrix(&camera);

        // CUBES
        renderManager.bindCubeVAO();
        glm::mat4 cubeMatrix;

        for (int i = -15; i < 15; i++)
        {            
            // Transformations
            cubeMatrix = glm::translate(*renderManager.getMVMatrix(), glm::vec3(i,0,-15));
            // On applique les transformations
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(*renderManager.getProjMatrix() * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            // We draw
            renderManager.getCubePtr()->drawCube();
        }
        for (int i = 0; i < 30; i++)
        {            
            cubeMatrix = glm::translate(*renderManager.getMVMatrix(), glm::vec3(-15,0,-14+i));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(*renderManager.getProjMatrix() * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            renderManager.getCubePtr()->drawCube();
        }
        for (int i = 0; i < 30; i++)
        {            
            cubeMatrix = glm::translate(*renderManager.getMVMatrix(), glm::vec3(14,0,-14+i));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(*renderManager.getProjMatrix() * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            renderManager.getCubePtr()->drawCube();
        }
        for (int i = -15; i < 15; i++)
        {            
            cubeMatrix = glm::translate(*renderManager.getMVMatrix(), glm::vec3(i,0,16));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(*renderManager.getProjMatrix() * cubeMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(cubeMatrix))));
            renderManager.getCubePtr()->drawCube();
        }
        
        renderManager.debindVAO();


        // SPHERES
        renderManager.bindSphereVAO();
        // On récupére la ViewMatrix à chaque tour de boucle
        glm::mat4 sphereMVMatrix = camera.getViewMatrix();
        // On applique les transformations
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(sphereMVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(sphereMVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(*renderManager.getProjMatrix() * sphereMVMatrix));
         // We draw
        renderManager.getSpherePtr()->drawSphere();
        renderManager.debindVAO();

        // Update the display
        windowManager.swapBuffers();
    }

    // FREE RESSOURCES
    // See ~renderManager destructor

    return EXIT_SUCCESS;
}