#version 460 core
out vec4 FragColor;

uniform vec3 light;
uniform vec3 color;

void main()
{
    FragColor = vec4(light * color, 1.0);
}