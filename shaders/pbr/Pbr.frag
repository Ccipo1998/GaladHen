
// PBR fragment shader

#version 460 core

subroutine vec3 ShadingMode();
layout (location = 0) subroutine uniform ShadingMode CurrentShadingMode;

subroutine vec3 DiffuseColorMode();
layout (location = 1) subroutine uniform DiffuseColorMode CurrentDiffuseColorMode;

subroutine vec3 NormalMode();
layout (location = 2) subroutine uniform NormalMode CurrentNormalMode;

subroutine float MetallicMode();
layout (location = 3) subroutine uniform MetallicMode CurrentMetallicMode;

subroutine float RoughnessMode();
layout (location = 4) subroutine uniform RoughnessMode CurrentRoughnessMode;

// outputs
out vec4 color;

// inputs
in vec3 SmoothWNormal;
flat in vec3 FlatWNormal;
in vec3 WPosition;
in vec3 WViewDirection;
in vec2 TexCoord;
in mat3 TBN;

// structs
struct PointLight
{
    float[3] Position;
    float[3] Color;
    float Intensity;
    float FallOffDistance;
};

struct DirectionalLight
{
    float[3] Position;
    float[3] Color;
    float Intensity;
    float[3] Direction;
};

// buffers
layout(std430, binding = 0) buffer PointLightsBuffer
{
    uint PointLightsNumber;
    PointLight PointLights[];
};

layout(std430, binding = 1) buffer DirectionalLightsBuffer
{
    // directional lights
    uint DirectionalLightsNumber;
    DirectionalLight DirectionalLights[];
};

// uniforms
// material
uniform vec3 DiffuseColor;
uniform vec3 Specular;
uniform float Metallic;
uniform float Roughness;
// matrix
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
// textures
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalMap;
uniform sampler2D MetallicTexture;
uniform sampler2D RoughnessTexture;

// const
const float pi = 3.141592653589793;
const float e = 0.0001; // tiny value to avoid dividing per zero
const vec3 dielectricsF0 = vec3(0.04);
const float gamma = 2.2;

// functions

vec3 GammaCorrection(vec3 shading)
{
    vec3 newShading = shading / (shading + vec3(1.0));
    newShading = pow(newShading, vec3(1.0/gamma));

    return newShading;
}

vec3 ColorToLinearSpace(vec3 srgbColor)
{
    return pow(srgbColor, vec3(gamma));
}

// Lambertian Reflectance
vec3 DiffuseBRDF(vec3 diffuseColor, float metallic)
{
    return diffuseColor / pi * (1.0 - metallic);
}

vec3 FresnelSchlickApprox(vec3 lightDir, vec3 halfDir, float metallic, vec3 diffuseColor)
{
    vec3 F0 = dielectricsF0 * (1.0 - metallic) + diffuseColor * metallic;
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

vec3 SpecularBRDF(vec3 viewNormal, vec3 lightDir, vec3 viewDir, vec3 halfDir, vec3 diffuseColor, float metallic, float roughness)
{
    vec3 fresnel = FresnelSchlickApprox(lightDir, halfDir, metallic, diffuseColor);
    float ggxDistribution = GGXNormalDistribution(viewNormal, halfDir, roughness);
    float ggxGeometry = GGXGeometryMasking(viewNormal, lightDir, viewDir, roughness);

    float NdotL = max(dot(viewNormal, lightDir), 0.0);
    float NdotV = max(dot(viewNormal, viewDir), 0.0);
    return (fresnel * ggxGeometry * ggxDistribution) / (4.0 * NdotL * NdotV + e);
}

// functions

// subroutines

layout (index = 0)
subroutine(ShadingMode)
vec3 SmoothShading()
{
    return normalize(SmoothWNormal);
}

layout (index = 1)
subroutine(ShadingMode)
vec3 FlatShading()
{
    return normalize(FlatWNormal);
}

layout (index = 2)
subroutine(DiffuseColorMode)
vec3 DiffuseColorConstant()
{
    return DiffuseColor;
}

layout (index = 3)
subroutine(DiffuseColorMode)
vec3 DiffuseColorSampling()
{
    return texture(DiffuseTexture, TexCoord).rgb;
}

layout (index = 4)
subroutine(NormalMode)
vec3 NormalInterpolated()
{
    return CurrentShadingMode();
}

layout (index = 5)
subroutine(NormalMode)
vec3 NormalSampling()
{
    vec3 normalSample = texture(NormalMap, TexCoord).rgb;
    normalSample = normalSample * 2.0 - 1.0;
    normalSample = normalize(TBN * normalSample);
    return normalSample;
}

layout (index = 6)
subroutine(MetallicMode)
float MetallicConstant()
{
    return Metallic;
}

layout (index = 7)
subroutine(MetallicMode)
float MetallicSampling()
{
    return texture(MetallicTexture, TexCoord).r;
}

layout (index = 8)
subroutine(RoughnessMode)
float RoughnessConstant()
{
    return Roughness;
}

layout (index = 9)
subroutine(RoughnessMode)
float RoughnessSampling()
{
    return texture(RoughnessTexture, TexCoord).r;
}

vec3 PhysicallyBasedShadingModel(vec3 wNormal, vec3 diffuseColor, float metallic, float roughness, vec3 nor)
{
    vec3 outgoing = vec3(0.0);

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 wLightPos = vec3(0.0);
    vec3 wLightDir = vec3(0.0);
    vec3 wHalfDir = vec3(0.0);

    // point lights
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        diffuse = DiffuseBRDF(diffuseColor, metallic);
        wLightPos = vec3(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2]);
        wLightDir = normalize(wLightPos - WPosition);
        wHalfDir = normalize(wLightDir + WViewDirection);
        specular = SpecularBRDF(wNormal, wLightDir, WViewDirection, wHalfDir, diffuseColor, metallic, roughness);
        outgoing += PointLights[i].Intensity * (diffuse + specular) * max(dot(nor, wLightDir), 0.0);
    }

    // directional lights
    for (uint i = 0; i < DirectionalLightsNumber; ++i)
    {
        diffuse = DiffuseBRDF(diffuseColor, metallic);
        wLightDir = -vec3(DirectionalLights[i].Direction[0], DirectionalLights[i].Direction[1], DirectionalLights[i].Direction[2]);
        //viewLightDir = (NormalMatrix * vec4(viewLightDir, 1.0)).xyz;
        wHalfDir = normalize(wLightDir + WViewDirection);
        specular = SpecularBRDF(wNormal, wLightDir, WViewDirection, wHalfDir, diffuseColor, metallic, roughness);
        outgoing += DirectionalLights[i].Intensity * (diffuse + specular) * max(dot(nor, wLightDir), 0.0);
    }

    return outgoing * pi;
}

// subroutines

void main()
{
    // take PBR parameters
    vec3 diffuse = CurrentDiffuseColorMode();
    vec3 normal = CurrentNormalMode();
    float metallic = CurrentMetallicMode();
    float roughness = CurrentRoughnessMode();

    // shading
    vec3 shading = PhysicallyBasedShadingModel(normal, diffuse, metallic, roughness, normal);

    // gamma correction
    shading = GammaCorrection(shading);

    color = vec4(shading, 1.0);
}
