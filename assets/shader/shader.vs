#version 400 core

in vec3 Position;
in vec2 TexCoords;

out vec3 XColor;
out vec2 XTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(Position, 1.0);
    //gl_Position = modelMatrix * vec4(Position, 1.0);
    XColor = vec3(0.0, 1.0, 0.0);
    XTexCoords = TexCoords;
}