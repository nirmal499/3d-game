#version 400 core

in vec3 XColor;
in vec2 XTexCoords;

out vec4 color;

uniform sampler2D textureSampler;

void main(void)
{
    color = texture(textureSampler, XTexCoords);
}