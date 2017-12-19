#include "glimac/Shader.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>

namespace glimac {

bool Shader::compile() {
	glCompileShader(m_nGLId);
	GLint status;
	glGetShaderiv(m_nGLId, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}

const std::string Shader::getInfoLog() const {
	GLint length;
	glGetShaderiv(m_nGLId, GL_INFO_LOG_LENGTH, &length);
	char* log = new char[length];
	glGetShaderInfoLog(m_nGLId, length, 0, log);
	std::string logString(log);
	delete [] log;
	return logString;
}

Shader loadShader(GLenum type, const FilePath& filepath) {
    std::ifstream input(filepath.c_str());
    if(!input) {
        throw std::runtime_error("Unable to load the file " + filepath.str());
    }
    
    std::stringstream buffer;
    buffer << input.rdbuf();
    
    Shader shader(type);
    shader.setSource(buffer.str().c_str());

    return shader;
}

}
