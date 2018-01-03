#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <ft2build.h>
#include <glimac/SDLWindowManager.hpp>
#include <GL/gl.h>
#include FT_FREETYPE_H

#include "project/RenderManager.hpp"
#include "project/GameManager.hpp"

#include "glimac/TrackballCamera.hpp"
#include "glimac/FreeflyCamera.hpp"
#include "project/Controller.hpp"

#include "project/Wall.hpp"
#include "project/GLSLProgram.hpp"

#include "project/Map.hpp"
#include "project/Texture.hpp"
#include "project/AudioManager.hpp"

#include "../../Code/glimac/include/glimac/glm.hpp"

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
    CubeMapProgram cubemapProgram(applicationPath);
    TextProgram textProgram(applicationPath);
    ProgramList programList;
    programList.normalProgram = &normalProgram;
    programList.textureProgram = &textureProgram;
    programList.cubemapProgram = &cubemapProgram;
    programList.textProgram = &textProgram;

    // Enable GPU depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Map map;
    map.setFileMap("classicMap.txt");
    //map.setFileMap("mapTest.txt");
    map.initialization();

    GameManager gameManager = GameManager(&map);
    gameManager.load(true);

    // Game Infos
    glm::vec2 gameSize = glm::vec2(gameManager.getMap()->getNbX(),gameManager.getMap()->getNbY());

    //TrackballCamera tpsCamera = TrackballCamera(gameSize.x,0,0.0f,0.0f);    // CAMERA VUE 2D
    TrackballCamera tpsCamera = TrackballCamera(gameSize.x,0,0.0f,-0.4f);
    FreeflyCamera fpsCamera = FreeflyCamera();
    Camera* camera = &tpsCamera;

    RenderManager renderManager = RenderManager(&windowManager, camera, &programList, gameSize);
    Controller controller = Controller(&windowManager);


    /* ---------------------
    *   INIT AUDIO | START MUSIC
    *  --------------------- */

    AudioManager audioManager = AudioManager();
    audioManager.addMusic(audioManager.createMusic("../Code/assets/audio/mainTheme.mp3"));
    audioManager.addMusic(audioManager.createMusic("../Code/assets/audio/mainThemeFast.mp3"));
    audioManager.fillSounds();
    audioManager.playMusic(0);

    // Load Textures
    renderManager.loadTextures();

    // Load font
    //renderManager.loadFont();
    renderManager.loadFont();

    // initialize Skybox
    renderManager.initSkybox();

    // initialize the timers
    gameManager.setTimers();

    windowManager.updateDeltaTime();

    // Application loop:a
    bool done = false;
    while(!done) {

        windowManager.updateDeltaTime();
        gameManager.updateSpeed(windowManager.getDeltaTime());

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
        tpsCamera.cameraController(&controller);
        fpsCamera.setCameraOnCharacter(map.getPacman(), gameSize);     // NEED TO FIX HERE !!
        gameManager.play(&controller, &audioManager);

        // Switch Camera mini-function
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

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // On update la ViewMatrix à chaque tour de boucle
        renderManager.updateMVMatrix(camera, map.getPacman());

        // // --- SPHERE --- //
        // // Bind Sphere VAO
        // renderManager.bindSphereVAO();
        // // Draw Pacman only in TPS
        // if(!controller.isFPSactive())
        //     renderManager.drawPacman(map.getPacman(), TEXTURE);
        //
        // renderManager.drawPacGommes(map.getPacGommes(), TEXTURE);
        // renderManager.drawSuperPacGommes(map.getSuperPacGommes(), TEXTURE);
        // renderManager.drawFruits(map.getFruits(), TEXTURE);
        //
        // // De-bind Sphere VAO
        // renderManager.debindVAO();
        //
        // // --- CUBE --- //T
         renderManager.bindCubeVAO();
         renderManager.drawSkybox();
         renderManager.debindVAO();

         // TEXT
         renderManager.drawText();

        // Update the display
        windowManager.swapBuffers();
    }

    // FREE RESSOURCES
    // See ~renderManager destructor

    return EXIT_SUCCESS;

    return 0;
}
