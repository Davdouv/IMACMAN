#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "project/RenderManager.hpp"
#include "project/GameManager.hpp"

#include "glimac/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "project/Controller.hpp"

#include "project/Wall.hpp"
#include "project/GLSLProgram.hpp"

#include "project/Map.hpp"

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

    // Create Programs (1 fragment shader = 1 program)
    FilePath applicationPath(argv[0]);
    NormalProgram normalProgram(applicationPath);
    TextureProgram textureProgram(applicationPath);
    ProgramList programList;
    programList.normalProgram = &normalProgram;
    programList.textureProgram = &textureProgram;

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    std::cout << windowManager.getDeltaTime() << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Map map;
    map.setFileMap("classicMap.txt");
    //map.setFileMap("mapTest.txt");
    map.initialization();

    GameManager gameManager = GameManager(&map);

    // Game Infos
    glm::vec2 gameSize = glm::vec2(map.getNbX(),map.getNbY());

    TrackballCamera tbCamera = TrackballCamera(gameSize.x,0,0.0f,1.57f);    // CAMERA VUE 2D
    //TrackballCamera tbCamera = TrackballCamera(gameSize.x,0,0.0f,1.f);
    FreeflyCamera fpsCamera = FreeflyCamera();
    Camera* camera = &tbCamera;

    RenderManager renderManager = RenderManager(&windowManager, camera, &programList, gameSize);
    Controller controller = Controller(&windowManager);

    // Load Textures
    renderManager.loadTextures();

    // initialize the ghosts
    gameManager.setGhosts();

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT)
            {
                done = true; // Leave the loop after this iteration
            }

            // Update controller with key & mouse events each frame
            controller.updateController(map.getPacman());
        }

        // Send the keys to the camera and the map
        tbCamera.cameraController(&controller);
        fpsCamera.setCameraOnCharacter(map.getPacman(), gameSize);     // NEED TO FIX HERE !!
        gameManager.play(&controller);

        // Switch Camera mini-function
        if (controller.getInterfaceAction() == Controller::C)
        {
            if(camera == &fpsCamera)
            {
                camera = &tbCamera;
                controller.setFPS(false);
            }
            else
            {
                camera = &fpsCamera;
                controller.setFPS(true);
            }
            controller.setInterfaceAction(Controller::NONE);
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // On update la ViewMatrix à chaque tour de boucle
        renderManager.updateMVMatrix(camera, map.getPacman());

        // --- SPHERE --- //
        // Bind Sphere VAO
        renderManager.bindSphereVAO();
        // Draw Pacman only in TPS
        if(!controller.isFPSactive())
            renderManager.drawPacman(map.getPacman(), TEXTURE);
        renderManager.drawPacGommes(map.getPacGommes(), TEXTURE);
        renderManager.drawSuperPacGommes(map.getSuperPacGommes(), TEXTURE);
        renderManager.drawFruits(map.getFruits(), TEXTURE);

        // De-bind Sphere VAO
        renderManager.debindVAO();

        // --- CUBE --- //
        renderManager.bindCubeVAO();

        renderManager.drawWalls(map.getWalls(), TEXTURE);
        renderManager.drawGhosts(map.getGhosts(), TEXTURE);

        renderManager.debindVAO();

        // Update the display
        windowManager.swapBuffers();
    }

    // FREE RESSOURCES
    // See ~renderManager destructor

    return EXIT_SUCCESS;
}
