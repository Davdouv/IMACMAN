#pragma once
#include <iostream>
#include <GL/glew.h>
#include "../glimac/glm.hpp"
#include "../glimac/Image.hpp"

#define GL_TEXTURE_CUBE_MAP_ARB             0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB  0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB  0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB  0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB  0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB  0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB  0x851A

class CubeMap {

private:
  GLuint m_id;
  std::string m_ImgFileFront;
  std::string m_ImgFileBack;
  std::string m_ImgFileTop;
  std::string m_ImgFileBottom;
  std::string m_ImgFileLeft;
  std::string m_ImgFileRight;
  std::unique_ptr<glimac::Image> m_ImgTextureFront;
  std::unique_ptr<glimac::Image> m_ImgTextureBack;
  std::unique_ptr<glimac::Image> m_ImgTextureTop;
  std::unique_ptr<glimac::Image> m_ImgTextureBottom;
  std::unique_ptr<glimac::Image> m_ImgTextureLeft;
  std::unique_ptr<glimac::Image> m_ImgTextureRight;

public:
    enum Side {FRONT = 0, BACK = 1, TOP = 2, BOTTOM = 3, LEFT = 4, RIGHT = 5};

    // constructor
    CubeMap(std::string ImgFileFront, std::string ImgFileBack, std::string ImgFileTop, std::string ImgFileBottom, std::string ImgFileLeft, std::string ImgFileRight);

    //getter
    GLuint getID() const;

    //methods
    bool loadCubeMapSide(GLuint texture, GLenum side_target, const std::string ImgFile);
    bool loadCubeMap();
};
