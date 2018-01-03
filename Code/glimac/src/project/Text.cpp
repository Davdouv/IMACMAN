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
    SDL_Surface * surface = TTF_RenderUTF8_Blended(font, text, fg);
    int colors = surface->format->BytesPerPixel;
    GLenum texture_format;
    if (colors == 4) {   // alpha
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA_EXT;
    } else {             // no alpha
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR_EXT;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  colors,
                  surface->w,
                  surface->h,
                  0,
                  texture_format,
                  GL_UNSIGNED_BYTE,
                  surface->pixels
                );
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void drawFont(SDL_Surface * surface, SDL_Surface * display)
  {
    SDL_Rect position;
    position.x = 60;
    position.y = 370;
    SDL_BlitSurface(surface, NULL, display, &position);
  }

  void clean()
  {
    TTF_Quit();
  }

}
