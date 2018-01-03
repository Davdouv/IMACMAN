#pragma once

#include "project/Texture.hpp"
#include "project/Player.hpp"

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