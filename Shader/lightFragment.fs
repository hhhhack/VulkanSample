#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 ViewPos;
uniform vec3 lightPos;
uniform vec3 light;
uniform vec3 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

void main()
{

    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    vec3 ambient = ambientStrength * light;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * light;

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diff * light;

    vec3 result = (ambient + diffuse + specular) * color;

    FragColor = vec4(result, 1.0);
}