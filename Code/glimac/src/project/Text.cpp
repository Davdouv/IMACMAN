#include "../include/project/Text.hpp"

namespace Text {

  void initText()
  {
    if (TTF_Init() < 0) {
        std::cout << "SDL_TTF library could not be load" << std::endl;
    }
  }

  TTF_Font* loadFont(const char* fontfile)
  {
    TTF_Font* font = TTF_OpenFont(fontfile, 48);
    if (font == NULL)
    {
      TTF_SetError("Loading failed :( (code: %d)", 142);
      std::cout << "Error: " << TTF_GetError() << std::endl;
      return NULL;
    }
    return font;
  }

  void clean()
  {
    TTF_Quit();
  }

}
