#version 410 core

in vec3 fragColor;       
in vec3 fragPosition;    

out vec4 fragmentColour;

uniform vec3 fogColor = vec3(0.5, 0.5, 0.5); 
uniform float density = 0.05;               
uniform float exponent = 2.0;                

void main() {
    float distance = abs(fragPosition.z);

    float fogFactor = exp(-pow(distance * density, exponent));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 finalColor = mix(fogColor, fragColor, fogFactor);

    fragmentColour = vec4(finalColor, 1.0);
}
