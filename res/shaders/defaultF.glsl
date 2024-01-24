#version 330 core

in vec2 tex;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D tex0;


void main()
{
    FragColor = texture2D(tex0, tex);
    //FragColor = vec4(1, 1, 1, 1);
}