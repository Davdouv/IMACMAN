#version 330 core

// Attributs de sommet
layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

// Matrices de transformations reçues en uniform
uniform mat4 uMVPMatrix;        // ModelViewProjection Matrix
uniform mat4 uMVMatrix;         // ModelView Matrix
uniform mat4 uNormalMatrix;     // Normalized Device Coordinates

// Sorties du shader
out vec3 vFragPosition; // Position du sommet transformé dans l'espace View
out vec3 vFragNormal; // Normale du sommet transformé dans l'espace View
out vec2 vFragTexCoords; // Coordonnées de texture du sommet


void main() {
   // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    // Calcul des valeurs de sortie
    vFragPosition = vec3(uMVMatrix * vertexPosition);
    vFragNormal = vec3(uNormalMatrix * vertexNormal);
    vFragTexCoords = aVertexTexCoords;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
};
