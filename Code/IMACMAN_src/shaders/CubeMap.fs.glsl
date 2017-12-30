#version 330

in vec3 vFragTexCoords;
uniform samplerCube uCubeTexture;

out vec4 fFragColor;

void main() {
  vec4 fFragColor = texture(uCubeTexture, vFragTexCoords);
}
