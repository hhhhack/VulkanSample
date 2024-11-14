#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform vec3 ViewPos;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse =light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);



    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}