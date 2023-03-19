#version 330 core

out vec4 FragColor;

struct Material
{
    vec3 diffuseColor;
};
uniform Material material;


void main()
{
	FragColor = vec4(material.diffuseColor,1.0);
}