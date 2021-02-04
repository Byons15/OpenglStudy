#version 330 core

in vec2 ourTexcoord;

out vec4 FragColor;  //ÑÕÉ«Êä³ö

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float factor;

void main()
{
	FragColor = mix(texture(texture1, ourTexcoord), texture(texture2, ourTexcoord), factor);
}