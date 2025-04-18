
// PBR fragment shader

// outputs
out vec4 color;

// inputs
in VS_OUT
{
    in vec3 SmoothWNormal;
    flat in vec3 FlatWNormal;
    in vec3 WPosition;
    in vec3 WViewDirection;
    in vec2 TexCoord;
    in mat3 TBN;
    in vec4 LightSpaceFragPos;
} vs_out;

// structs
struct PointLight
{
    vec4 Color;
    vec3 Position;
    float Intensity;
    float BulbSize;
    float Radius;
};

struct DirectionalLight
{
    vec4 Color;
    vec3 Position;
    float Intensity;
    vec3 Direction;
};

// buffers
layout(std140, binding = 0) buffer PointLightBuffer
{
    PointLight PointLights[];
};
layout(std140, binding = 1) buffer DirectionalLightBuffer
{
    DirectionalLight DirectionalLights[];
};

// uniforms
layout (std140, binding = 0) uniform CameraData
{
    uniform mat4 ViewMatrix;
    uniform mat4 ProjectionMatrix;
    uniform vec3 WCameraPosition;
};
layout (std140, binding = 2) uniform LightingData
{
    int PointLightNumber;
    int DirLightNumber;
};

// const
const float pi = 3.141592653589793;
const float epsilon = 0.0001; // tiny value to avoid dividing per zero
const vec4 dielectricsF0 = vec4(0.04, 0.04, 0.04, 1.0);

// Gamma correction
#include "GaladHen/Shaders/Common/GammaCorrection.glsl"

// Color space operations
#include "GaladHen/Shaders/Common/ColorSpace.glsl"

// Shadow mapping
#include "GaladHen/Shaders/Common/ShadowMapping.glsl"

float WindowedInverseSquareFalloff(float intensity, float lightRadius, float falloffDistance, float distanceFromLightSource)
{
    float intensityFalloff = intensity * (pow(lightRadius, 2.0) / (max(pow(distanceFromLightSource, 2.0), lightRadius) + epsilon));
    float windowingFunction = pow(max((1.0 - pow(distanceFromLightSource / (falloffDistance + epsilon), 4.0)), 0.0), 2.0);

    return intensityFalloff * windowingFunction;
}

// Lambertian Reflectance
vec4 DiffuseBRDF(vec4 diffuseColor, float metallic)
{
    return diffuseColor / pi * (1.0 - metallic);
}

vec4 FresnelSchlickApprox(vec3 lightDir, vec3 halfDir, float metallic, vec4 diffuseColor)
{
    vec4 F0 = dielectricsF0 * (1.0 - metallic) + diffuseColor * metallic;
    return F0 + (1.0 - F0) * pow((1.0 - (max(dot(lightDir, halfDir), 0.0))), 5.0);
}

float GGXNormalDistribution(vec3 viewNormal, vec3 halfDir, float roughness)
{
    float NdotH = max(dot(viewNormal, halfDir), 0.0);
    float powRoughness = pow(roughness, 4.0);
    return powRoughness / (pi * pow((NdotH * NdotH) * (powRoughness - 1.0) + 1.0, 2.0));
}

float GGXSmithMasking(vec3 viewNormal, vec3 dir, float roughness)
{
    float NdotD = max(dot(viewNormal, dir), 0.0);
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    return NdotD / ((NdotD) * (1.0 - k) + k);
}

float GGXGeometryMasking(vec3 viewNormal, vec3 lightDir, vec3 viewDir, float roughness)
{
    return GGXSmithMasking(viewNormal, lightDir, roughness) * GGXSmithMasking(viewNormal, viewDir, roughness);
}

vec4 SpecularBRDF(vec3 viewNormal, vec3 lightDir, vec3 viewDir, vec3 halfDir, vec4 diffuseColor, float metallic, float roughness)
{
    vec4 fresnel = FresnelSchlickApprox(lightDir, halfDir, metallic, diffuseColor);
    float ggxDistribution = GGXNormalDistribution(viewNormal, halfDir, roughness);
    float ggxGeometry = GGXGeometryMasking(viewNormal, lightDir, viewDir, roughness);

    float NdotL = max(dot(viewNormal, lightDir), 0.0);
    float NdotV = max(dot(viewNormal, viewDir), 0.0);
    return (fresnel * ggxGeometry * ggxDistribution) / (4.0 * NdotL * NdotV + epsilon);
}

// Functions to define
vec4 ComputeDiffuseColor();
vec3 ComputeNormal();
float ComputeMetallic();
float ComputeRoughness();

vec4 PhysicallyBasedShadingModel(vec3 wNormal, vec4 diffuseColor, float metallic, float roughness)
{
    vec4 outgoing = vec4(0.0);

    vec4 diffuse = vec4(0.0);
    vec4 specular = vec4(0.0);

    vec3 wLightPos = vec3(0.0);
    vec3 wLightDir = vec3(0.0);
    vec3 wLightPosDistance = vec3(0.0);
    vec3 wHalfDir = vec3(0.0);

    float lightIntensity = 0.0;
    float shadowTest = 0.0;

    // point lights
    for (uint i = 0; i < PointLightNumber; ++i)
    {
        diffuse = DiffuseBRDF(diffuseColor, metallic);
        wLightPos = PointLights[i].Position;
        wLightPosDistance = wLightPos - vs_out.WPosition;
        wLightDir = normalize(wLightPosDistance);
        wHalfDir = normalize(wLightDir + vs_out.WViewDirection);
        specular = SpecularBRDF(wNormal, wLightDir, vs_out.WViewDirection, wHalfDir, diffuseColor, metallic, roughness);
        lightIntensity = WindowedInverseSquareFalloff(PointLights[i].Intensity, PointLights[i].BulbSize, PointLights[i].Radius, length(wLightPosDistance));
        outgoing += lightIntensity * (diffuse + specular) * max(dot(wNormal, wLightDir), 0.0);
    }

    // directional lights
    for (uint i = 0; i < DirLightNumber; ++i)
    {
        diffuse = DiffuseBRDF(diffuseColor, metallic);
        wLightDir = -DirectionalLights[i].Direction;
        wHalfDir = normalize(wLightDir + vs_out.WViewDirection);
        specular = SpecularBRDF(wNormal, wLightDir, vs_out.WViewDirection, wHalfDir, diffuseColor, metallic, roughness);
        shadowTest = ShadowTest(vs_out.LightSpaceFragPos);
        outgoing += (1.0 - shadowTest) * DirectionalLights[i].Intensity * (diffuse + specular) * max(dot(wNormal, wLightDir), 0.0);
    }

    return outgoing * pi;
}

// subroutines

void main()
{
    // take PBR parameters
    vec4 diffuse = ComputeDiffuseColor();
    vec3 normal = ComputeNormal();
    float metallic = ComputeMetallic();
    float roughness = ComputeRoughness();

    // shading
    vec4 shading = PhysicallyBasedShadingModel(normal, diffuse, metallic, roughness);

    // gamma correction
    shading = GammaCorrection(shading);

    color = vec4(shading);
}
