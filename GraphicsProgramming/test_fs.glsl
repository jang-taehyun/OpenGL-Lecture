#version 430 core						

in vec3 vsNormal;
in vec3 vsPos;

out vec4 fragColor;

uniform vec3 viewPos;

struct Material
{
//	vec3 ambient;
//	vec3 diffuse;
	sampler2D diffuse;

//	vec3 specular;
	sampler2D specular;
	int shininess;
};
uniform Material material;
in vec2 vsTexCoord;

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;


void main(void)						
{
	// vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsTexCoord));

	vec3 norm = normalize(vsNormal);
	vec3 lightDir = normalize(light.position - vsPos);
	float diff = max(dot(norm, lightDir), 0.f);
	// vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vsTexCoord));

	vec3 viewDir = normalize(viewPos - vsPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	// vec3 specular = light.specular * spec * material.specular;
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vsTexCoord));

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.f);		
}										