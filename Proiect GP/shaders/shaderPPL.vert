#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 textcoord;


//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;


out vec3 fragPos;
out vec3 normal;
out vec2 passTexture;

void main()
{	
	
	fragPos = vec3(view * model * vec4(vPosition, 1.0));
    normal = normalize(normalMatrix * vNormal);
	passTexture = textcoord;
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
} 
