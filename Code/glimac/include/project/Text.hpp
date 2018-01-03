#pragma once
#include <ft2build.h>
#include <iostream>
#include <string>
#include <cmath>
#include <GL/glew.h>
#include <SDL/SDL_ttf.h>
#include "glimac/common.hpp"
#include "../glimac/glm.hpp"

#include FT_FREETYPE_H

using namespace glimac;

namespace Text {

  void initText();

  TTF_Font* loadFont(const char* fontfile, int ptsize);

  SDL_Surface* renderFont(TTF_Font *font, const char *text, SDL_Color fg);

  void drawFont(SDL_Surface * surface, SDL_Surface * display);

  void clean();

}
