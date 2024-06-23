#version 400 core

in vec2 XTexCoord;
in vec3 SurfaceNormal;
in vec3 ToLightVector;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 lightColour;

void main(void)
{
    vec3 UnitNormal = normalize(SurfaceNormal);
    vec3 UnitLightVector = normalize(ToLightVector);

    float NDot1 = dot(UnitNormal, UnitLightVector);
    float brightness = max(NDot1, 0.0);
    vec3 diffuse = brightness * lightColour;

    color = vec4(diffuse, 1.0) * texture(textureSampler, XTexCoord);
    // color = texture(textureSampler, XTexCoord);
}