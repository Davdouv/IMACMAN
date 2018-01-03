#include "project/Menu.hpp"

using namespace glimac;

Menu::Menu(bool mainMenu)
{
    m_button = PLAY;
    m_mainMenu = mainMenu;
    if (mainMenu)   // Main menu
    {
        m_texture[PLAY] = new Texture("../Code/assets/textures/play.png");
        m_texture[CONTINUE] = new Texture("../Code/assets/textures/continue.png");
        m_texture[EXIT] = new Texture("../Code/assets/textures/exit.jpg");
    }
    else    // Pause Menu
    {
        m_texture[0] = new Texture("../Code/assets/textures/play.png");
        m_texture[1] = new Texture("../Code/assets/textures/continue.png");
        m_texture[EXIT] = new Texture("../Code/assets/textures/exit.jpg");
    }
    m_texture[3] = new Texture("../Code/assets/textures/gum.jpg");  // Select Button
    m_texture[4] = new Texture("../Code/assets/textures/ghost.jpg");    // Background

    for (unsigned int i = 0; i < 5; ++i)
        m_texture[i]->loadTexture();
}

Menu::~Menu()
{
    for (unsigned int i = 0; i < 5; ++i)
        delete(m_texture[i]);
}

Texture** Menu::getTextures()
{
    return m_texture;
}

Menu::Button Menu::getButton() const
{
    return m_button;
}

bool Menu::isMainMenu() const
{
    return m_mainMenu;
}

void Menu::selectButton(Controller* controller, AudioManager* audioManager)
{
    if ((controller->getInterfaceAction() == Controller::Key::DOWN) || (controller->getInterfaceAction() == Controller::Key::UP))
    {
        audioManager->playSound(0,1);
        if (controller->getInterfaceAction() == Controller::Key::UP)
        {
            if(m_button == EXIT)
                m_button = CONTINUE;
            else if (m_button == CONTINUE)
                m_button = PLAY;
            controller->setInterfaceAction(Controller::Key::NONE);      
        }
        else
        {
            if(m_button == PLAY)
                m_button = CONTINUE;
            else if (m_button == CONTINUE)
                m_button = EXIT;
            controller->setInterfaceAction(Controller::Key::NONE);   
        }
    }
}