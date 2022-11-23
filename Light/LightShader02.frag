#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 texcoord;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform PointLight pointLight;

uniform vec3 viewPos;

vec3 calcDirLight()
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoord));

	// 漫射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, texcoord)));

	// 镜面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflecDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, texcoord)));

	vec3 result = ambient + diffuse + specular;
	return result;
}

vec3 calcPointLight()
{
	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, texcoord));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointLight.position - FragPos);

	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = pointLight.diffuse * (diff * vec3(texture(material.diffuse, texcoord)));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflecDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflecDir), 0.0), material.shininess);
	vec3 specular = pointLight.specular * (spec * vec3(texture(material.specular, texcoord)));

	float dis = length(pointLight.position - FragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dis + pointLight.quadratic * (dis * dis));

	return attenuation * (ambient + diffuse + specular);
}

void main()
{
	vec3 dirLight = calcDirLight();
	vec3 ptLight = calcPointLight();
	FragColor = vec4(dirLight + ptLight, 1.0);
}