#pragma once
#include <ft2build.h>
#include <iostream>
#include <string>
#include <cmath>
#include <GL/glew.h>
#include "../glimac/glm.hpp"

#include FT_FREETYPE_H

class Text {
private:
  GLuint m_vbo;
  FT_Library m_ft;
  FT_Face m_face;
  std::string m_fontfilename;

public:
  FT_Face getFace() { return m_face; }
  GLuint getVBO() { return m_vbo; }
  void init_resources();
  void render_text(const char *text, float x, float y, float sx, float sy);
};
