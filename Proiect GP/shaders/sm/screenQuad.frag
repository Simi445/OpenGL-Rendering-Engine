#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D sceneTexture;  // Main scene texture (with shadows)
uniform sampler2D bloomTexture;  // Blurred bright parts
uniform float bloomIntensity;    // Controls the strength of the bloom effect

void main() 
{
    // Sample the scene and bloom textures
    vec4 sceneColor = texture(sceneTexture, fTexCoords);
    vec4 bloomColor = texture(bloomTexture, fTexCoords);

    // Combine the scene and bloom with adjustable intensity
    fColor = sceneColor + 0.2*bloomIntensity * bloomColor;
}
