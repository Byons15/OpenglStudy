#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 texcoord;

out vec4 FragColor;

uniform Material material;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	// 环境光
	vec3 ambient = lightColor * vec3(texture(material.diffuse, texcoord)) * 0.1;

	// 漫射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * vec3(texture(material.diffuse, texcoord)));

	// 镜面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflecDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * vec3(texture(material.specular, texcoord)));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}