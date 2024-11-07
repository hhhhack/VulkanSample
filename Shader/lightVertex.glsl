#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
void main()
{
    gl_Position = trans * view * model * vec4(aPos, 1.0f);
    Normal = aNormal;
}
