#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D specular;
uniform sampler2D specular2;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	FragColor = texture(diffuse1, TexCoords);
}