#version 460 core
out vec4 FragColor;

flat in int flagUseUniform;
uniform vec4 realColor;

void main()
{
    if (flagUseUniform == 1){
        FragColor = vec4(0.5f, 0.1f, 0.0f, 1.0f);
    }else{
        FragColor = realColor;
    }
}