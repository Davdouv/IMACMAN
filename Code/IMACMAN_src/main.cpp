#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "project/RenderManager.hpp"

#include "glimac/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "project/Controller.hpp"

#include "project/Wall.hpp"
#include "project/GLSLProgram.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    // Default window size
    glm::vec2 defaultWindowSize = glm::vec2(1280,720);

    // Initialize SDL and open a window
    SDLWindowManager windowManager(defaultWindowSize.x, defaultWindowSize.y, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    // Create Programs
    FilePath applicationPath(argv[0]);
    NormalProgram normalProgram(applicationPath);
    ProgramList programList;
    programList.normalProgram = &normalProgram;

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Game Infos
    glm::vec2 gameSize = glm::vec2(30,30);
    glm::vec2 gameCorner = glm::vec2(-(gameSize.x / 2), -(gameSize.y / 2));

    //TrackballCamera tbCamera = TrackballCamera(30,0,0.0f,1.57f);    // CAMERA VUE 2D
    TrackballCamera tbCamera = TrackballCamera(30,0,0.0f,1.0f);
    RenderManager renderManager = RenderManager(&windowManager, &tbCamera, &programList, gameSize);
    Controller controller = Controller(&windowManager);


    Wall wall1(0,0);
    Wall wall2(30,0);
    Wall wall3(0,30);
    Wall wall4(30,30);

    // Enable program
    renderManager.useProgram(NORMAL);

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

        tbCamera.cameraController(&controller);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // On récupére la ViewMatrix à chaque tour de boucle
        renderManager.updateMVMatrix(&tbCamera);
        glm::mat4 viewMatrix = renderManager.getMVMatrix();

        // SPHERE
        renderManager.bindSphereVAO();
        // On applique les transformations
        renderManager.applyTransformations(NORMAL, viewMatrix);
         // We draw
        renderManager.getSpherePtr()->drawSphere();
        renderManager.debindVAO();

        // WALL TEST
        renderManager.bindCubeVAO();
        glm::mat4 wallMatrix;

        // Wall 1
        wallMatrix = renderManager.translateToPosition(wall1.getPosX(), wall1.getPosY());
        renderManager.applyTransformations(NORMAL, wallMatrix);
        renderManager.getCubePtr()->drawCube();

        // Wall 2
        wallMatrix = renderManager.translateToPosition(wall2.getPosX(), wall2.getPosY());
        renderManager.applyTransformations(NORMAL, wallMatrix);
        renderManager.getCubePtr()->drawCube();

        // Wall 3
        wallMatrix = renderManager.translateToPosition(wall3.getPosX(), wall3.getPosY());
        renderManager.applyTransformations(NORMAL, wallMatrix);
        renderManager.getCubePtr()->drawCube();

        // Wall 4
        wallMatrix = renderManager.translateToPosition(wall4.getPosX(), wall4.getPosY());
        renderManager.applyTransformations(NORMAL, wallMatrix);
        renderManager.getCubePtr()->drawCube();

        renderManager.debindVAO();

        // Update the display
        windowManager.swapBuffers();
    }

    // FREE RESSOURCES
    // See ~renderManager destructor

    return EXIT_SUCCESS;
}