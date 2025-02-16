#version 410 core

// Vertex Shader
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoord;

out vec2 fragTexCoord;
out vec3 fragPosition;
out vec3 fragNormal;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;

void main() 
{
    fragTexCoord = texCoord;
    fragPosition = vec3(view * model * vec4(vertexPosition, 1.0));
    fragNormal = normalize(normalMatrix * vertexNormal);
    gl_Position = projection * vec4(fragPosition, 1.0);
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vertexPosition, 1.0f);
}