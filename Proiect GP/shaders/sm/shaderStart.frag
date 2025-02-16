#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
out vec4 fColor;


//texture
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


//fog
uniform vec3 fogColor = vec3(0.9, 0.9, 0.9);
uniform float density = 0.1;
uniform float exponent = 8;


//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;


vec3 ambient;
uniform float ambientStrength = 2.2f;
vec3 diffuse;
vec3 specular;
uniform float specularStrength = 0.2f;
uniform float shininess = 2.0f;

//pentru lumina
uniform float localWeight = 0.4; 
uniform float globalWeight = 0.6; 
uniform bool isAlpha = false;

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
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
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
	
	float bias = max(0.005f * (1.0f - dot(fNormal, normalize(lightDir))), 0.0005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

	return shadow;
}


void main() 
{
	vec3 ambient1, diffuse1, specular1;
    computeLightComponents(lightSource1, fPosEye.xyz, fNormal, ambient1, diffuse1, specular1);

    // // Light source 2 components
    vec3 ambient2, diffuse2, specular2;
    computeLightComponents(lightSource2, fPosEye.xyz, fNormal, ambient2, diffuse2, specular2);

	computeGobalComponents();
	
	vec3 texColor = texture(diffuseTexture, fTexCoords).rgb;
    vec3 specColor = texture(specularTexture, fTexCoords).rgb;
	
	ambient *= texColor;
	diffuse *= texColor;
	specular *= specColor;


	//Pana aici e ok!
	ambient1 *= texColor;
    diffuse1 *= texColor;
    specular1 *= specColor;

    ambient2 *= texColor;
    diffuse2 *= texColor;
    specular2 *= specColor;

    ambient *= texColor;  // Global ambient light
    diffuse *= texColor;  // Global diffuse light
    specular *= specColor; // Global specular light
	
	
	float shadow = computeShadow();

	vec3 lightingLocal = (((ambient1 + diffuse1)* 3/4) + ((ambient2 + diffuse2) * 1/4)) * texColor + (specular1 + specular2*0.75);
	
	vec3 lightingGlobal =  min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

    vec3 lighting = localWeight * lightingLocal + globalWeight * lightingGlobal;

	float distance = length(fPosEye); // Use Euclidean distance
    float fogFactor = exp(-pow(distance * density, exponent));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 finalColor = mix(fogColor, lighting, fogFactor);

    fColor = isAlpha ? vec4(finalColor, 0.5f) : vec4(finalColor, 1.0f);
}

