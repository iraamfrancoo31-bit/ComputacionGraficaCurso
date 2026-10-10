
#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light light2;
uniform sampler2D texture_diffuse1;

vec3 calculateLight(Light source, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(source.position - FragPos);

    vec3 ambient = source.ambient * material.ambient;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = source.diffuse * diff * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        material.shininess
    );

    vec3 specular = source.specular * spec * material.specular;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result =
        calculateLight(light, norm, viewDir) +
        calculateLight(light2, norm, viewDir);

    vec4 texColor = texture(texture_diffuse1, TexCoords);
    color = vec4(result, 1.0) * texColor;
}
