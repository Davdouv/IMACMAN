#version 330

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;// Coordonées de texture

out vec3 fFragColor;

// Variable uniforme
uniform sampler2D uTexture;
uniform float uTime;

void main() {
    vec2 TexCoords = vec2(vFragTexCoords.x, vFragTexCoords.y-uTime);
    vec4 textureColor = texture(uTexture, TexCoords);

    fFragColor = vec3(textureColor.r, textureColor.g, textureColor.b);
};