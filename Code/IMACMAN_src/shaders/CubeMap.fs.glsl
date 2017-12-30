#version 330

in vec3 vFragTexCoords;

uniform samplerCube cubeTexture;

out vec4 fFragColor;

void main () {
   vec4 cubeMapColour = texture (cubeTexture, vFragTexCoords);
   fFragColor = cubeMapColour;
}
