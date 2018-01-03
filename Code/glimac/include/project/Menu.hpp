#pragma once

#include "project/Texture.hpp"
#include "project/Controller.hpp"
#include "project/AudioManager.hpp"

using namespace glimac;

class Menu {

public:
    enum Button {PLAY = 0, CONTINUE = 1, EXIT = 2};

private:
    Button m_button;
    Texture* m_texture[5];
    bool m_mainMenu;

public:
    //Menu();
    Menu(bool mainMenu);
    ~Menu();

    // Getters
    Texture** getTextures();
    Button getButton() const;
    bool isMainMenu() const;
    // Setter for button
    void selectButton(Controller* controller, AudioManager* audioManager);
};