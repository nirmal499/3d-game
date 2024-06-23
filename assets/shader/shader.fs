#version 400 core

in vec2 XTexCoord;
in vec3 SurfaceNormal;
in vec3 ToLightVector;
in vec3 ToCameraVector;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

void main(void)
{
    vec3 UnitNormal = normalize(SurfaceNormal);
    vec3 UnitLightVector = normalize(ToLightVector);

    float diffuseFactor = dot(UnitNormal, UnitLightVector);
    diffuseFactor = max(diffuseFactor, 0.2);
    vec3 diffuse = diffuseFactor * lightColour;

    vec3 UnitToCameraVector = normalize(ToCameraVector);
    vec3 LightDirection = -UnitLightVector;
    vec3 ReflectedLightDirection = reflect(LightDirection, UnitNormal);

    float specularFactor = dot(ReflectedLightDirection, UnitToCameraVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 specular = dampedFactor * reflectivity * lightColour;

    color = vec4(diffuse, 1.0) * texture(textureSampler, XTexCoord) + vec4(specular, 1.0);
}