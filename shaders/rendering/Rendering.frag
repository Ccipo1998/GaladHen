
// Fragment shader for rendering.
// This shader offers many types of rendering

#version 460 core

// subroutine declarations

subroutine vec3 ShadingMode();
layout (location = 0) subroutine uniform ShadingMode CurrentShadingMode;
subroutine vec3 ShadingType(vec3 viewNormal);
layout (location = 1) subroutine uniform ShadingType CurrentShadingType;

// outputs
out vec4 color;

// inputs
in vec3 SmoothViewNormal;
flat in vec3 FlatViewNormal;
in vec3 ViewPosition;
in vec3 ViewDirection;

// structs
struct PointLight
{
    float[3] Color;
    float Intensity;
    float[3] Position;
    float FallOffDistance;
};

// buffers
layout(std430, binding = 0) buffer PointLightsBuffer
{
    uint PointLightsNumber;
    PointLight PointLights[];
};

// uniforms
// material
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float Metallic;
uniform float Roughness;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float SpecularFalloff;
// matrix
uniform mat4 ViewMatrix;

// functions

vec3 PhongAmbient(float lightIntensity)
{
    return Ka * lightIntensity;
}

vec3 PhongDiffuse(vec3 viewNormal, float lightIntensity, vec3 lightDir)
{
    // diffuse reflection equation
    return Kd * lightIntensity * max(dot(viewNormal, lightDir), 0.0); // WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular
}

vec3 PhongSpecular(vec3 viewNormal, float lightIntensity, vec3 lightDir)
{
    vec3 specularDir = normalize(-lightDir + 2 * dot(lightDir, viewNormal) * viewNormal);
    return Ks * lightIntensity * pow(max(dot(specularDir, ViewDirection), 0.0), SpecularFalloff);
}

// functions

// subroutines

layout (index = 0)
subroutine(ShadingMode)
vec3 SmoothShading()
{
    return SmoothViewNormal;
}

layout (index = 1)
subroutine(ShadingMode)
vec3 FlatShading()
{
    return FlatViewNormal;
}

layout (index = 2)
subroutine(ShadingType)
vec3 PhongDiffuseReflection(vec3 viewNormal)
{
    // compute diffuse reflection adding contribution from all the lights
    vec3 phongDiffuse = vec3(0.0);
    vec3 viewLightPos = vec3(0.0);
    vec3 lightDir = vec3(0.0);
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        lightDir = normalize(viewLightPos - ViewPosition);
        phongDiffuse += PhongDiffuse(viewNormal, PointLights[i].Intensity, lightDir);
    }

    return Diffuse * phongDiffuse;
}

layout (index = 3)
subroutine(ShadingType)
vec3 PhongShadingModel(vec3 viewNormal)
{
    // compute phong shading model adding contribution from all the lights
    vec3 phongAmbient = vec3(0.0);
    vec3 phongDiffuse = vec3(0.0);
    vec3 phongSpecular = vec3(0.0);
    vec3 viewLightPos = vec3(0.0);
    vec3 lightDir = vec3(0.0);
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        // ambient
        phongAmbient += PhongAmbient(PointLights[i].Intensity);

        // diffuse
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        lightDir = normalize(viewLightPos - ViewPosition);
        phongDiffuse += PhongDiffuse(viewNormal, PointLights[i].Intensity, lightDir);

        // specular
        phongSpecular += PhongSpecular(viewNormal, PointLights[i].Intensity, lightDir);
    }
    
    // create color
    vec3 color = Diffuse * phongAmbient + Diffuse * phongDiffuse + phongSpecular;

    return color;
}

// subroutines

void main()
{
    vec3 viewNormal = CurrentShadingMode();
    vec3 diffuseColor = CurrentShadingType(viewNormal);
    color = vec4(diffuseColor, 1.0);
}
