#version 430 core						
										
in vec3 vsColor;
in vec3 vsNormal;
in vec3 vsPos;

out vec4 fragColor;

uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 viewPos;
uniform int shininess;

uniform vec3 lightPos;
										
void main(void)						
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(vsNormal);
	vec3 lightDir = normalize(lightPos - vsPos);
	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - vsPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularStrength = 0.5f;
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vsColor;

	fragColor = vec4(result, 1.0);		
}										