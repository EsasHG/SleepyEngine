#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse1;

void main()
{
	FragColor = texture(diffuse1, TexCoords.st);
	//FragColor = vec4(TexCoords, 0.0,1.0);
}