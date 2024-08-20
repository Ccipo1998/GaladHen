
// PBR fragment shader

#version 450 core

// standard subroutine
subroutine vec3 ShadingMode();
layout (location = 0) subroutine uniform ShadingMode GetShadingMode;

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

// material data
uniform vec4 Diffuse;
uniform vec4 Specular;
uniform float Metallic;
uniform float Roughness;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D MetallicTexture;
uniform sampler2D RoughnessTexture;

// const
const float pi = 3.141592653589793;
const float epsilon = 0.0001; // tiny value to avoid dividing per zero
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

float WindowedInverseSquareFalloff(float intensity, float lightRadius, float falloffDistance, float distanceFromLightSource)
{
    float intensityFalloff = intensity * (pow(lightRadius, 2.0) / (max(pow(distanceFromLightSource, 2.0), lightRadius) + epsilon));
    float windowingFunction = pow(max((1.0 - pow(distanceFromLightSource / (falloffDistance + epsilon), 4.0)), 0.0), 2.0);

    return intensityFalloff * windowingFunction;
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
    return (fresnel * ggxGeometry * ggxDistribution) / (4.0 * NdotL * NdotV + epsilon);
}

// functions

// subroutines

subroutine vec3 DiffuseMode();
layout (location = 1) subroutine uniform DiffuseMode GetDiffuse;

subroutine vec3 NormalMode();
layout (location = 2) subroutine uniform NormalMode GetNormal;

subroutine float MetallicMode();
layout (location = 3) subroutine uniform MetallicMode GetMetallic;

subroutine float RoughnessMode();
layout (location = 4) subroutine uniform RoughnessMode GetRoughness;

layout (index = 0)
subroutine(ShadingMode)
vec3 SmoothShading()
{
    return normalize(vs_out.SmoothWNormal);
}

layout (index = 1)
subroutine(ShadingMode)
vec3 FlatShading()
{
    return normalize(vs_out.FlatWNormal);
}

layout (index = 2)
subroutine(DiffuseMode)
vec3 DiffuseConstant()
{
    return Diffuse.rgb;
}

layout (index = 3)
subroutine(DiffuseMode)
vec3 DiffuseSampling()
{
    return texture(DiffuseTexture, vs_out.TexCoord).rgb;
}

layout (index = 4)
subroutine(NormalMode)
vec3 NormalInterpolated()
{
    return GetShadingMode();
}

layout (index = 5)
subroutine(NormalMode)
vec3 NormalSampling()
{
    vec3 normalSample = texture(NormalTexture, vs_out.TexCoord).rgb;
    normalSample = normalSample * 2.0 - 1.0;
    normalSample = normalize(vs_out.TBN * normalSample);
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
    return texture(MetallicTexture, vs_out.TexCoord).r;
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
    return texture(RoughnessTexture, vs_out.TexCoord).r;
}

vec3 PhysicallyBasedShadingModel(vec3 wNormal, vec3 diffuseColor, float metallic, float roughness)
{
    vec3 outgoing = vec3(0.0);

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 wLightPos = vec3(0.0);
    vec3 wLightDir = vec3(0.0);
    vec3 wLightPosDistance = vec3(0.0);
    vec3 wHalfDir = vec3(0.0);

    float lightIntensity = 0.0;

    // point lights
    for (uint i = 0; i < 1; ++i)
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
    for (uint i = 0; i < DirectionalLights.length(); ++i)
    {
        diffuse = DiffuseBRDF(diffuseColor, metallic);
        wLightDir = -DirectionalLights[i].Direction;
        wHalfDir = normalize(wLightDir + vs_out.WViewDirection);
        specular = SpecularBRDF(wNormal, wLightDir, vs_out.WViewDirection, wHalfDir, diffuseColor, metallic, roughness);
        outgoing += DirectionalLights[i].Intensity * (diffuse + specular) * max(dot(wNormal, wLightDir), 0.0);
    }

    return outgoing * pi;
}

// subroutines

void main()
{
    // take PBR parameters
    vec3 diffuse = GetDiffuse();
    vec3 normal = GetNormal();
    float metallic = GetMetallic();
    float roughness = GetRoughness();

    // shading
    vec3 shading = PhysicallyBasedShadingModel(normal, diffuse, metallic, roughness);

    // gamma correction
    shading = GammaCorrection(shading);

    color = vec4(shading, 1.0);
}
