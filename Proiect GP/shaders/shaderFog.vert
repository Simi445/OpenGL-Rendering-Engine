#version 410 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fragColor;
out vec3 fragPosition; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragColor = vertexColor;
    fragPosition = (view * model * vec4(vertexPosition, 1.0)).xyz;
    gl_Position = projection * vec4(fragPosition, 1.0);
}
