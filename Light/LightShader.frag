#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrenght;

void main()
{
	//计算光源向量，规范化两个向量
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	//计算点积，修正点积值，乘上光源颜色值
	float diff = max(dot(norm, lightDir), 0.0);  //负数没有意义
	vec3 diffuse = diff * lightColor;
	FragColor = vec4(objectColor * (lightColor * (ambientStrenght + diffuse)), 1.0f);
}