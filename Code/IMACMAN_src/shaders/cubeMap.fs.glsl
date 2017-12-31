#version 330

in vec3 vFragTexCoords;

uniform samplerCube cubeTexture;

out vec4 fFragColor;

void main () {
  fFragColor = texture(cubeTexture, vFragTexCoords);
}
