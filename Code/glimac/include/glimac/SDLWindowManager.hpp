#pragma once

#include <cstdint>
#include <SDL/SDL.h>
#include "glm.hpp"

namespace glimac {

class SDLWindowManager {
public:
    SDLWindowManager(uint32_t width, uint32_t height, const char* title);

    ~SDLWindowManager();

    bool pollEvent(SDL_Event& e);

    bool isKeyPressed(SDLKey key) const;

    // button can SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE
    bool isMouseButtonPressed(uint32_t button) const;

    glm::ivec2 getMousePosition() const;

    void swapBuffers();

    // Return the time in seconds
    float getTime() const;

    // Delta Time (using SDL_GetTicks())
    uint32_t getDeltaTime() const;
    void updateDeltaTime();

    float getRatio() const;

    uint32_t getWidth() const;
    uint32_t getHeight() const;

    void setWidth(uint32_t width);
    void setHeight(uint32_t height);

    SDL_Surface* getScreen() { return m_screen; }

private:
    uint32_t m_width;
    uint32_t m_height;

    uint32_t lastTickTime;
    uint32_t deltaTime;

    SDL_Surface* m_screen;
};

}
