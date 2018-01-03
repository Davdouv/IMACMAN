#include "../include/project/Text.hpp"

namespace Text {

  void initText()
  {
    if (TTF_Init() < 0) {
        std::cout << "SDL_TTF library could not be load" << std::endl;
    }
  }

  TTF_Font* loadFont(const char* fontfile, int ptsize)
  {
    TTF_Font* font = TTF_OpenFont(fontfile, ptsize);
    if (font == NULL)
    {
      TTF_SetError("Loading failed :( (code: %d)", 142);
      std::cout << "Error: " << TTF_GetError() << std::endl;
      return NULL;
    }
    return font;
  }

  SDL_Surface* renderFont(TTF_Font *font, const char *text, SDL_Color fg)
  {
    SDL_Surface * surface = TTF_RenderText_Blended(font, text, fg);
  }

  void drawFont(SDL_Surface * surface, SDL_Surface * display)
  {
    SDL_BlitSurface(surface, NULL, display, NULL);
  }

  void clean()
  {
    TTF_Quit();
  }

}
