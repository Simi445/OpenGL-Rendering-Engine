#version 410 core

// Fragment Shader
in vec2 fragTexCoord;
in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragPosLightSpace;
out vec4 fragmentColour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap; 


struct Light {
    vec3 lightPosEye;
    vec3 lightColor;
    vec3 cameraPosEye;

    float ambientStrength;
    float specularStrength;
    float shininess;

    float constant;
    float linear;
    float quadratic;
};

uniform Light lightSource1;
uniform Light lightSource2;

// Fog parameters
uniform vec3 fogColor = vec3(0.5, 0.5, 0.5);
uniform float density = 0.02;
uniform float exponent = 1.5;


//global lighting components
uniform	vec3 lightDir;
uniform	vec3 lightColor;

vec3 ambient;
float ambientStrength = 0.1f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.4f;
float shininess = 32.0f;

void computeLightComponents(Light lightSource, vec3 fragPosition, vec3 fragNormal, out vec3 ambient, out vec3 diffuse, out vec3 specular) {
    float dist = length(lightSource.lightPosEye - fragPosition);
    float att = 1.0f / (lightSource.constant + lightSource.linear * dist + lightSource.quadratic * (dist * dist));

    vec3 normalEye = normalize(fragNormal);
    vec3 lightDirN = normalize(lightSource.lightPosEye - fragPosition);
    vec3 viewDirN = normalize(lightSource.cameraPosEye - fragPosition);
    vec3 halfVector = normalize(lightDirN + viewDirN);

    ambient = lightSource.ambientStrength * lightSource.lightColor;

    float diff = max(dot(normalEye, lightDirN), 0.0f);
    diffuse = att * diff * lightSource.lightColor;

    float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), lightSource.shininess);
    specular = att * lightSource.specularStrength * specCoeff * lightSource.lightColor;
}



void computeGobalComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);
	
	//transform normal
	vec3 normalEye = normalize(fragNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosition);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}



float computeShadow()
{
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	if (normalizedCoords.z > 1.0f)
		return 0.0f;

	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	float currentDepth = normalizedCoords.z;
	
	float bias = max(0.05f * (1.0f - dot(fragNormal, normalize(lightDir))), 0.005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

	return shadow;
}

void main() {
    // Light source 1 components
    vec3 ambient1, diffuse1, specular1;
    computeLightComponents(lightSource1, fragPosition, fragNormal, ambient1, diffuse1, specular1);

    // // Light source 2 components
    vec3 ambient2, diffuse2, specular2;
    computeLightComponents(lightSource2, fragPosition, fragNormal, ambient2, diffuse2, specular2);

    computeGobalComponents();

    // Apply textures independently to each light source
    vec3 texColor = texture(diffuseTexture, fragTexCoord).rgb;
    vec3 specColor = texture(specularTexture, fragTexCoord).rgb;

    ambient1 *= texColor;
    diffuse1 *= texColor;
    specular1 *= specColor;

    ambient2 *= texColor;
    diffuse2 *= texColor;
    specular2 *= specColor;

    ambient *= texColor;  // Global ambient light
    diffuse *= texColor;  // Global diffuse light
    specular *= specColor; // Global specular light


    // Shadow computation
    float shadow = computeShadow();

    // Compute shadowed lighting

    //vec3 lightingLocal = (((ambient1 + diffuse1)* 3/4) + ((ambient2 + diffuse2) * 1/4)) * texColor + (specular1 + specular2*0.75);

    vec3 lighting =  min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

    // float localWeight = 0.7; // Give more weight to local lighting
    // float globalWeight = 0.3; // Less weight to global lighting
    // vec3 lighting = localWeight * lightingLocal + globalWeight * lightingGlobal;

    // Fog calculation
    // float distance = length(fragPosition); // Use Euclidean distance
    // float fogFactor = exp(-pow(distance * density, exponent));
    // fogFactor = clamp(fogFactor, 0.0, 1.0);
    // vec3 finalColor = mix(fogColor, lighting, fogFactor);

    //fragmentColour = vec4(finalColor, 1.0);
    fragmentColour = vec4(lighting, 1.0);
}
