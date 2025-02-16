#version 410 core

// Inputs from the vertex shader
in vec3 fragPos;
in vec3 normal;
in vec2 passTexture;  

out vec4 fColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 baseColor;

uniform sampler2D textureSampler;

void main()
{
    vec3 textureColor = texture(textureSampler, passTexture).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDirN = normalize(lightDir);
    vec3 viewDir = normalize(-fragPos);
    
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    float diff = max(dot(norm, lightDirN), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDirN, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 lighting = (ambient + diffuse) * textureColor + specular;

    fColor = vec4(min(lighting, vec3(1.0)), 1.0);
}
