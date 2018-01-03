#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "project/RenderManager.hpp"
#include "project/GameManager.hpp"
// #include "project/AudioManager.hpp"
#include "project/Controller.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"

#include "project/GLSLProgram.hpp"

using namespace glimac;

int main(int argc, char** argv) {

    /* -------------
    *   INIT WINDOW
    *  ------------- */

    // Default window size
    glm::vec2 defaultWindowSize = glm::vec2(1280,720);

    // Initialize SDL and open a window
    SDLWindowManager windowManager(defaultWindowSize.x, defaultWindowSize.y, "IMACMAN");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    /* -------------
    *   INIT PROGRAMS
    *  ------------- */

    // Create Programs (1 fragment shader = 1 program)
    FilePath applicationPath(argv[0]);
    NormalProgram normalProgram(applicationPath);
    TextureProgram textureProgram(applicationPath);
    CubeMapProgram cubemapProgram(applicationPath);
    DirectionnalLightProgram directionnalLightProgram(applicationPath);
    PointLightProgram pointLightProgram(applicationPath);

    ProgramList programList;
    programList.normalProgram = &normalProgram;
    programList.textureProgram = &textureProgram;
    programList.cubemapProgram = &cubemapProgram;
    programList.directionnalLightProgram = &directionnalLightProgram;
    programList.pointLightProgram = &pointLightProgram;

    /* --------------------------
    *   INIT GAME and CONTROLLER
    *  -------------------------- */

    Map map;
    map.setFileMap("classicMap.txt");
    map.initialization();
    glm::vec2 gameSize = glm::vec2(map.getNbX(),map.getNbY());

    GameManager gameManager = GameManager(&map);

    Controller controller = Controller(&windowManager);

    /* --------------
    *   INIT CAMERAS
    *  -------------- */

    //TrackballCamera tpsCamera = TrackballCamera(gameSize.x,0,0.0f,0.0f);    // CAMERA VIEW "2D"
    TrackballCamera tpsCamera = TrackballCamera(gameSize.x,0,0.0f,-0.4f);
    FreeflyCamera fpsCamera = FreeflyCamera();
    Camera* camera = &tpsCamera;

    /* ---------------------
    *   INIT RENDER MANAGER
    *  --------------------- */

    RenderManager renderManager = RenderManager(&windowManager, camera, &programList, gameSize);

    // Load Textures
    renderManager.loadTextures();
    // initialize Skybox
    renderManager.initSkybox();

    /* ---------------------
    *   INIT AUDIO | NOT WORKING YET. NEED TO LINK THE LIBRARY
    *  --------------------- */

    //AudioManager audioManager = AudioManager();

    /* -------------
    *   INIT TIME
    *  ------------- */

    // initialize the timers
    gameManager.setTimers();
    // Time & Delta Time
    windowManager.updateDeltaTime();
    gameManager.setStartTime(SDL_GetTicks());

    /* ------------------------------------------------------------
    *   APPLICATION LOOP | 1.EVENTS | 2.GAME ENGINE | 3. RENDERING
    *  ------------------------------------------------------------ */

    bool done = false;
    while(!done) {

        /* ------------------
        *   UPDATE DELTA TIME
        *  ------------------ */

        windowManager.updateDeltaTime();
        gameManager.updateSpeed(windowManager.getDeltaTime());

        /* ------------------
        *   EVENT LOOP
        *  ------------------ */

        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT)
            {
                done = true; // Leave the loop after this iteration
            }

            // Update controller with key & mouse events each frame
            controller.updateController(map.getPacman());
        }

        /* ------------------
        *   MANAGE CAMERAS
        *  ------------------ */

        // Send the keys to the camera and the map
        tpsCamera.cameraController(&controller);
        // Ask the camera to track pacman
        fpsCamera.setCameraOnCharacter(map.getPacman(), gameSize);
        // Switch Camera FPS / TPS if C button is pressed
        if (controller.getInterfaceAction() == Controller::C)
        {
            if(camera == &fpsCamera)
            {
                camera = &tpsCamera;
                controller.setFPS(false);
            }
            else
            {
                camera = &fpsCamera;
                controller.setFPS(true);
            }
            controller.setInterfaceAction(Controller::NONE);
        }

        /* --------------------------------------------------------------------------
        *   PLAY FUNCTION : Move characters, Check for collision, Update Player infos
        *  -------------------------------------------------------------------------- */

        gameManager.play(&controller);

        /* ------------------
        *   RENDERING CODE
        *  ------------------ */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update The View Matrix each time we enter the while loop
        renderManager.updateMVMatrix(camera, map.getPacman());
        // Render the map (objects, skybox and ground)
        renderManager.drawMap(&map, &controller);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;

    return 0;
}
