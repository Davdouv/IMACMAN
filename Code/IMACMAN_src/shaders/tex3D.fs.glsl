#version 330

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;// Coordonées de texture

out vec3 fFragColor;

// Variable uniforme
uniform sampler2D uTexture;

void main() {
    vec4 textureColor = texture(uTexture, vFragTexCoords);

    fFragColor = vec3(textureColor.r, textureColor.g, textureColor.b);
};