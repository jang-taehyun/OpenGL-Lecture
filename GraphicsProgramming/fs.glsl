#version 430 core

//#define MULTIPLE_LIGHT
#define SEVERAL
//#define DIRECTIONAL_LIGHT
#define POINT_LIGHT
//#define SPOT_LIGHT

// -------------------- several lighting -------------------- //
#ifdef SEVERAL

in vec3 vsNormal;
in vec3 vsPos;
in vec2 vsTexCoord;

out vec4 fragColor;

uniform vec3 viewPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};
uniform Material material;

struct Light
{
#ifdef DIRECTIONAL_LIGHT
	vec3 direction;
#endif

#ifdef POINT_LIGHT
	vec3 position;
	float c1, c2;
#endif

#ifdef SPOT_LIGHT
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCufOff;
	float c1, c2;
#endif

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;


void main(void)						
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));

	vec3 norm = normalize(vsNormal);

#ifdef DIRECTIONAL_LIGHT
	vec3 lightDir = normalize(-light.direction);
#endif

#ifdef POINT_LIGHT
	vec3 lightDir = normalize(light.position - vsPos);
	float dist = length(light.position - vsPos);
	float attenuation = 1.f / (1.f + light.c1 * dist + light.c2 * dist * dist);
#endif

#ifdef SPOT_LIGHT
	vec3 lightDir = normalize(light.position - vsPos);

	float dist = length(light.position - vsPos);
	float attenuation = 1.f / (1.f + light.c1 * dist + light.c2 * dist * dist);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCufOff;
	float intensity = attenuation * clamp((theta - light.outerCufOff) / epsilon, 0.f, 1.f);
#endif

	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));

	vec3 viewDir = normalize(viewPos - vsPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

	vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

#ifdef DIRECTIONAL_LIGHT
	vec3 result = ambient + diffuse + specular;
#endif

#ifdef POINT_LIGHT
	vec3 result = (ambient + diffuse + specular) * attenuation;
#endif

#ifdef SPOT_LIGHT
	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.f);

	// there is no soft edge //	
//	if(theta > light.cutOff)
//	{
//		vec3 result = ambient + diffuse + specular;
//		fragColor = vec4(result, 1.f);
//	}
//	else
//		fragColor = vec4(ambient, 1.f);
#else
	fragColor = vec4(result, 1.f);
#endif
}

#endif

// -------------------- multiple lighting -------------------- //
#ifdef MULTIPLE_LIGHT

#define NUM_POINT_LIGHTS 2

in vec3 vsNormal;
in vec3 vsPos;
in vec2 vsTexCoord;

out vec4 fragColor;

uniform vec3 viewPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};
uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight
{
	vec3 position;

	float c1, c2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float c1, c2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)						
{
	vec3 norm = normalize(vsNormal);
	vec3 viewDir = normalize(viewPos - vsPos);

	// directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	// point lighting
	for(int i=0; i<NUM_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, vsPos, viewDir);
	}

	// spot lighting
	result += CalcSpotLight(spotLight, norm, vsPos, viewDir);

	fragColor = vec4(result, 1.f);
}

// directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.f);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

	return (ambient + diffuse + specular);
}

// point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.f);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.f / (1.f + light.c1 * distance + light.c2 * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

	return (ambient + diffuse + specular) * attenuation;
}

// spot light
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.f);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);

	// soft edge spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.f);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.f / (1.f + light.c1 * distance + light.c2 * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

#endif