#version 450 core
layout(location = 0) in vec3 aPos;

out int flagUseUniform;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    flagUseUniform = 1;
}