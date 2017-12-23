#include "../include/project/Texture.hpp"


Texture::Texture(std::string ImgFile) : m_id(0), m_ImgFile(ImgFile) {
}

GLuint Texture::getID() const { return m_id; }

void Texture::setImgFile(const std::string &ImgFile) { m_ImgFile = ImgFile; }

bool Texture::loadTexture() {
  m_ImgTexture = glimac::loadImage(m_ImgFile);

  if(m_ImgTexture == 0)
    {
        std::cout << "Error : cannot load texture image" << std::endl;
        return false;
    }

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     m_ImgTexture->getWidth(),
                     m_ImgTexture->getHeight(),
                     0,
                     GL_RGBA,
                     GL_FLOAT,
                     m_ImgTexture->getPixels()
                     );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}
