#version 400 core

in vec3 Position;
in vec2 TexCoords;

out vec3 XColor;
out vec2 XTexCoords;

void main()
{
    gl_Position = vec4(Position, 1.0);
    XColor = vec3(0.0, 1.0, 0.0);
    XTexCoords = TexCoords;
}