#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sceneTexture;       
uniform sampler2D bloomTexture;       
uniform float bloomIntensity;         

void main() {
    vec3 scene = texture(sceneTexture, TexCoords).rgb;  // Main scene
    vec3 bloom = texture(bloomTexture, TexCoords).rgb;  // Bloom texture

    // Combine the two textures
    vec3 result = scene + bloom * bloomIntensity;

    FragColor = vec4(result, 1.0); 
}