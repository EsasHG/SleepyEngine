#version 330 core

out vec4 FragColor;

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D specular1;
uniform sampler2D specular2;

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
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLight;

uniform vec3 viewPos;


in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;

} fs_in;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0),32);

    vec3 ambient = light.ambient * vec3(texture(diffuse1,fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse1, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular1, fs_in.TexCoords));

    return (ambient + diffuse + specular);

}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32); //shininess

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic * (distance*distance));

    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse1, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular1, fs_in.TexCoords));
    
    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(0.0,0.0,0.0);
    result += CalculateDirLight(dirLight, norm, viewDir);
    result += CalculatePointLight(pointLight, norm, fs_in.FragPos, viewDir);
	FragColor = vec4(result, 1.0);
}