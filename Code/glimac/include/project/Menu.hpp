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
    Texture* m_texture[8];
    bool m_mainMenu;
    bool m_hide;
    bool m_infos;
    
    void switchHide();
    void switchInfos();

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
    bool isHidden() const { return m_hide; }
    bool getInfos() const { return m_infos; }
};