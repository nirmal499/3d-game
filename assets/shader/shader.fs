#version 400 core

in vec3 XColor;

out vec4 color;

void main(void)
{
    color = vec4(XColor, 1.0);
}