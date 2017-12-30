#include "glimac/SDLWindowManager.hpp"
#include <iostream>

namespace glimac {

SDLWindowManager::SDLWindowManager(uint32_t width, uint32_t height, const char* title) {
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    if(!SDL_SetVideoMode(width, height, 32, SDL_OPENGL)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    SDL_WM_SetCaption(title, nullptr);
    m_width = width;
    m_height = height;
    lastTickTime = 0;
    deltaTime = 0;
}

SDLWindowManager::~SDLWindowManager() {
    SDL_Quit();
}

bool SDLWindowManager::pollEvent(SDL_Event& e) {
    return SDL_PollEvent(&e);
}

bool SDLWindowManager::isKeyPressed(SDLKey key) const {
    return SDL_GetKeyState(nullptr)[key];
}

// button can SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE
bool SDLWindowManager::isMouseButtonPressed(uint32_t button) const {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

glm::ivec2 SDLWindowManager::getMousePosition() const {
    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return mousePos;
}

void SDLWindowManager::swapBuffers() {
    SDL_GL_SwapBuffers();
}

float SDLWindowManager::getTime() const {
    return 0.001f * SDL_GetTicks();
}

uint32_t SDLWindowManager::getDeltaTime() {
    return deltaTime;
}

void SDLWindowManager::updateDeltaTime()
{
    uint32_t tickTime = SDL_GetTicks();
    deltaTime = tickTime - lastTickTime;
    lastTickTime = tickTime;
}

float SDLWindowManager::getRatio() const
{
    return (float)m_width/m_height;
}

uint32_t SDLWindowManager::getWidth() const
{
    return m_width;
}
uint32_t SDLWindowManager::getHeight() const
{
    return m_height;
}

void SDLWindowManager::setWidth(uint32_t width)
{
    m_width = width;
}
void SDLWindowManager::setHeight(uint32_t height)
{
    m_height = height;
}

}
