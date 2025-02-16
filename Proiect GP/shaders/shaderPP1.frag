#version 410 core

out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D sceneTexture;



void main() {
    vec3 color = texture(sceneTexture, TexCoords).rgb;
	float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // Luminance
    if(brightness > 0.96)
        BrightColor = vec4(color, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    
}
