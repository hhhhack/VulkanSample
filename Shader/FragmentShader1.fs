#version 450 core
out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord);
}