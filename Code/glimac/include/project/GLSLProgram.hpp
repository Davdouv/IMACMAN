#pragma once

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

namespace glimac {

// Every key player can use
enum FS {NORMAL, TEXTURE, CUBEMAP, MULTITEXTURE, DIRECTIONNAL_LIGHT, POINT_LIGHT};

// GLSL Program with the normal fragment shader
struct NormalProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    NormalProgram(){}
    NormalProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/normal.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
    }
};

// GLSL Program with the 3D texture fragment shader
struct TextureProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    TextureProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

// GLSL Program with the Cubemap texture fragment shader
struct CubeMapProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint cubeTexture;

    CubeMapProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/cubeMap.vs.glsl",
                              applicationPath.dirPath() + "shaders/cubeMap.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        cubeTexture = glGetUniformLocation(m_Program.getGLId(), "cubeTexture");
    }
};

// GLSL Program with the Multi 3D texture fragment shader
struct MultiTextureProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture1;
    GLint uTexture2;

    MultiTextureProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture1 = glGetUniformLocation(m_Program.getGLId(), "uTexture1");
        uTexture2 = glGetUniformLocation(m_Program.getGLId(), "uTexture2");
    }
};

// GLSL Program with the Directionnal Light fragment shader
struct DirectionnalLightProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightDir_vs;
    GLint uLightIntensity;

    DirectionnalLightProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");

        // Variables uniformes materiaux
        GLint uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        GLint uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        GLint uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        // Variables uniformes lumieres
        GLint uLightDir_vs = glGetUniformLocation(m_Program.getGLId(), "uLightDir_vs");
        GLint uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

// GLSL Program with the Point Light fragment shader
struct PointLightProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint uKd;
    GLint uKs;
    GLint uShininess;
    GLint uLightPos_vs;
    GLint uLightIntensity;

    PointLightProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/pointlight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");

        // Variables uniformes materiaux
        GLint uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
        GLint uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
        GLint uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        // Variables uniformes lumieres
        GLint uLightPos_vs = glGetUniformLocation(m_Program.getGLId(), "uLightPos_vs");
        GLint uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

struct ProgramList {
    NormalProgram* normalProgram;
    TextureProgram* textureProgram;
    MultiTextureProgram* multiTextureProgram;
    DirectionnalLightProgram* directionnalLightProgram;
    PointLightProgram* pointLightProgram;
    CubeMapProgram* cubemapProgram;
};

}
