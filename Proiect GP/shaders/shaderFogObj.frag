#version 410 core

in vec2 fragTexCoord;
in vec3 fragPosition;

out vec4 fragmentColour;

uniform sampler2D diffuseTexture;  // texture sampler
uniform vec3 fogColor = vec3(0.5, 0.5, 0.5); // fog color
uniform float density = 0.05; // controls fog density
uniform float exponent = 2.0; // controls fall-off rate

void main() {
    // texture color for the object
    vec3 texColor = texture(diffuseTexture, fragTexCoord).rgb;

    // distance to the fragment
    float distance = abs(fragPosition.z);

    //fog factor
    float fogFactor = exp(-pow(distance * density, exponent));
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // texture color with the fog color
    vec3 finalColor = mix(fogColor, texColor, fogFactor);

    //output
    fragmentColour = vec4(finalColor, 1.0);
}
