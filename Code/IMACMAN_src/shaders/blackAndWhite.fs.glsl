#version 330

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;// Coordonées de texture

out vec3 fFragColor;

// COLOR
// INFOS TEXTURE
uniform sampler2D uTexture;
// INFOS MATERIAUX
uniform vec3 uKd;               // coefficient de reflection diffuse de l'objet
uniform vec3 uKs;               // coefficient de reflection glossy de l'objet
uniform float uShininess;       // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy
// INFOS LUMIERE
uniform vec3 uLightDir_vs;      // suffixe _vs pour viewSpace // Direction de la lumiere
uniform vec3 uLightIntensity;   // intensite de la lumiere
// il faudra multiplier la direction de la lumière par la View Matrix avant de l'envoyer au shader.


// Fonction qui calcule et renvoie la couleur du fragment en fonction des paramètres de matériau et d'éclairage
vec3 blinnPhong(vec3 Kd, vec3 Ks, float shininess, vec3 lightDirection, vec3 Li, vec3 N)
{
    // wi correspond simplement à uLightDir_vs normalisé
    vec3 wi = normalize(lightDirection);

    // wo est simplement l'opposé de la position du fragment normalisée (normalize(-vPosition_vs))
    vec3 wo = normalize(-vFragPosition);

    vec3 halfVector = normalize((wo+wi)/2);

    float glossyVec = pow(dot(halfVector, N), shininess);

    vec3 color = Li*(Kd*dot(wi,N)+(Ks * glossyVec));

    return color;
}


void main() {
    vec4 textureColor = texture(uTexture, vFragTexCoords);
    //fFragColor = blinnPhong(uKd, uKs, uShininess, uLightDir_vs, uLightIntensity, normalize(vFragNormal))*vec3(textureColor);
    fFragColor = blinnPhong(uKd, uKs, uShininess, uLightDir_vs, uLightIntensity, normalize(vFragNormal)) * vec3(textureColor);
    fFragColor = vec3((fFragColor.r+fFragColor.g+fFragColor.b)/3);
}