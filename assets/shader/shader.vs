#version 400 core

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
 
out vec2 XTexCoord;
out vec3 SurfaceNormal;
out vec3 ToLightVector;
out vec3 ToCameraVector;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
    vec4 WorldPosition = modelMatrix * vec4(Position, 1.0);

    gl_Position = projectionMatrix * viewMatrix * WorldPosition;
    
    XTexCoord = TexCoord;

    SurfaceNormal = (modelMatrix * vec4(Normal, 0.0)).xyz;
    ToLightVector = lightPosition - WorldPosition.xyz;

    ToCameraVector = cameraPosition - WorldPosition.xyz;
    
}