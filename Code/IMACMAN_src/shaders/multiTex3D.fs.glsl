#version 330

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;// Coordonées de texture

out vec3 fFragColor;

// Variable uniforme
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {
    vec4 textureColor1 = texture(uEarthTexture, vFragTexCoords);
    vec4 textureColor2 = texture(uCloudTexture, vFragTexCoords);
    vec4 textureColor = textureColor1 + textureColor2;

    fFragColor = (textureColor).rgb;
};