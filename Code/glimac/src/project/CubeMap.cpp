#include "../include/project/CubeMap.hpp"


CubeMap::CubeMap(std::string ImgFileFront, std::string ImgFileBack, std::string ImgFileTop, std::string ImgFileBottom, std::string ImgFileLeft, std::string ImgFileRight)
: m_id(0), m_ImgFileFront(ImgFileFront), m_ImgFileBack(ImgFileBack), m_ImgFileTop(ImgFileTop), m_ImgFileBottom(ImgFileBottom), m_ImgFileLeft(ImgFileLeft), m_ImgFileRight(ImgFileRight) {
}

GLuint CubeMap::getID() const { return m_id; }

bool CubeMap::loadCubeMap() {
  glGenTextures(1, &m_id);

  // load each image and copy into a side of the cube-map texture
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_ImgFileFront);
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_ImgFileBack);
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_ImgFileTop);
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_ImgFileBottom);
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_ImgFileLeft);
  loadCubeMapSide(m_id, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_ImgFileRight);
  // format cube map texture
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return true;
}

bool loadCubeMapSide(GLuint texture, GLenum side_target, const std::string ImgFile) {
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

  int x, y, n;
  int force_channels = 4;
  std::unique_ptr<glimac::Image> ImgTexture = glimac::loadImage(ImgFile);
  if (!ImgTexture) {
    std::cerr << "ERROR: could not load" << ImgFile << std::endl;
    return false;
  }
  // non-power-of-2 dimensions check
  if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
    std::cerr << "WARNING: image" << ImgFile << "is not power-of-2 dimensions" << std::endl;
  }

  // copy image data into 'target' side of cube map
  glTexImage2D( side_target,
                0,
                GL_RGBA,
                ImgTexture->getWidth(),
                ImgTexture->getHeight(),
                0,
                GL_RGBA,
                GL_FLOAT,
                ImgTexture->getPixels()
              );

  return true;
}
