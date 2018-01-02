#include "../include/project/Texture.hpp"


Texture::Texture() : m_id(0), m_ImgFile("") {
}

Texture::Texture(std::string ImgFile) : m_id(0), m_ImgFile(ImgFile) {
}

GLuint Texture::getID() const { return m_id; }

void Texture::setImgFile(const std::string &ImgFile) { m_ImgFile = ImgFile; }

bool Texture::loadTexture() {
  if (m_ImgFile != "")
  {
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
  }

  // RENDER TARGET TEXTURE
  else
  {
      glGenTextures(1, &m_id);
      glBindTexture(GL_TEXTURE_2D, m_id);

      glTexImage2D(GL_TEXTURE_2D,
                        0,
                        GL_RGB,
                        1024,
                        768,
                        0,
                        GL_RGB,
                        GL_FLOAT,
                        0
                        );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glBindTexture(GL_TEXTURE_2D, 0);

      // Set "renderedTexture" as our colour attachement #0
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_id, 0);

      // Set the list of draw buffers.
      GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
      glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

      // Always check that our framebuffer is ok
      if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
  }

  return true;
}
