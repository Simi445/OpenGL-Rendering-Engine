#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 texCoord; // for objects using textures

out vec2 fragTexCoord;
out vec3 fragPosition; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragTexCoord = texCoord;
    fragPosition = (view * model * vec4(vertexPosition, 1.0)).xyz;
    gl_Position = projection * vec4(fragPosition, 1.0);
}
