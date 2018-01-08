#pragma once

#include "project/Texture.hpp"
#include "project/Player.hpp"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

using namespace glimac;

class UI {

private:
    Texture* m_texture[1];
    Player* m_player;

public:
    UI(Player*);
    ~UI();

    // Getters
    Texture** getTextures();
    Player* getPlayer();
};
