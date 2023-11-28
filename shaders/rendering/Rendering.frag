
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

struct DirectionalLight
{
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
uniform mat4 NormalMatrix;

// const
const float pi = 3.141592653589793;
const float e = 0.0001; // tiny value to avoid dividing per zero
const vec3 dielectricsF0 = vec3(0.04);

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

vec3 BlinnPhongSpecular(vec3 viewNormal, float lightIntensity, vec3 lightDir)
{
    vec3 halfDir = normalize(ViewDirection + lightDir);
    return Ks * lightIntensity * pow(max(dot(halfDir, viewNormal), 0.0), SpecularFalloff);
}

// Lambertian Reflectance
vec3 DiffuseBRDF()
{
    return Diffuse / pi * (1.0 - Metallic);
}

vec3 FresnelSchlickApprox(vec3 lightDir, vec3 halfDir)
{
    vec3 F0 = dielectricsF0 * (1.0 - Metallic) + Diffuse * Metallic;
    return F0 + (1.0 - F0) * pow((1.0 - (max(dot(lightDir, halfDir), 0.0))), 5.0);
}

float GGXNormalDistribution(vec3 viewNormal, vec3 halfDir)
{
    float NdotH = max(dot(viewNormal, halfDir), 0.0);
    float powRoughness = pow(Roughness, 4.0);
    return powRoughness / (pi * pow((NdotH * NdotH) * (powRoughness - 1.0) + 1.0, 2.0));
}

float GGXSmithMasking(vec3 viewNormal, vec3 dir)
{
    float NdotD = max(dot(viewNormal, dir), 0.0);
    float k = pow(Roughness + 1.0, 2.0) / 8.0;
    return NdotD / ((NdotD) * (1.0 - k) + k);
}

float GGXGeometryMasking(vec3 viewNormal, vec3 lightDir, vec3 viewDir)
{
    return GGXSmithMasking(viewNormal, lightDir) * GGXSmithMasking(viewNormal, viewDir);
}

vec3 SpecularBRDF(vec3 viewNormal, vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
    vec3 fresnel = FresnelSchlickApprox(lightDir, halfDir);
    float ggxDistribution = GGXNormalDistribution(viewNormal, halfDir);
    float ggxGeometry = GGXGeometryMasking(viewNormal, lightDir, viewDir);

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
    vec3 viewLightDir = vec3(0.0);

    // point lights
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        viewLightDir = normalize(viewLightPos - ViewPosition);
        phongDiffuse += PhongDiffuse(viewNormal, PointLights[i].Intensity, viewLightDir);
    }

    // directional lights
    for (uint i = 0; i < DirectionalLightsNumber; ++i)
    {
        viewLightDir = vec3(DirectionalLights[i].Direction[0], DirectionalLights[i].Direction[1], DirectionalLights[i].Direction[2]);
        viewLightDir = (NormalMatrix * vec4(viewLightDir, 1.0)).xyz;
        phongDiffuse += PhongDiffuse(viewNormal, DirectionalLights[i].Intensity, viewLightDir);
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
    vec3 viewLightDir = vec3(0.0);

    // point lights
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        // ambient
        phongAmbient += PhongAmbient(PointLights[i].Intensity);

        // diffuse
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        viewLightDir = normalize(viewLightPos - ViewPosition);
        phongDiffuse += PhongDiffuse(viewNormal, PointLights[i].Intensity, viewLightDir);

        // specular
        phongSpecular += PhongSpecular(viewNormal, PointLights[i].Intensity, viewLightDir);
    }

    // directional lights
    for (uint i = 0; i < DirectionalLightsNumber; ++i)
    {
        // ambient
        phongAmbient += PhongAmbient(DirectionalLights[i].Intensity);
        
        viewLightDir = vec3(DirectionalLights[i].Direction[0], DirectionalLights[i].Direction[1], DirectionalLights[i].Direction[2]);
        viewLightDir = (NormalMatrix * vec4(viewLightDir, 1.0)).xyz;
        phongDiffuse += PhongDiffuse(viewNormal, DirectionalLights[i].Intensity, viewLightDir);

        // specular
        phongSpecular += PhongSpecular(viewNormal, DirectionalLights[i].Intensity, viewLightDir);
    }
    
    // create color
    vec3 color = Diffuse * phongAmbient + Diffuse * phongDiffuse + phongSpecular;

    return color;
}

layout (index = 4)
subroutine(ShadingType)
vec3 BlinnPhongShadingModel(vec3 viewNormal)
{
    // compute phong shading model adding contribution from all the lights
    vec3 phongAmbient = vec3(0.0);
    vec3 phongDiffuse = vec3(0.0);
    vec3 blinnPhongSpecular = vec3(0.0);
    vec3 viewLightPos = vec3(0.0);
    vec3 viewLightDir = vec3(0.0);

    // point lights
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        // ambient
        phongAmbient += PhongAmbient(PointLights[i].Intensity);

        // diffuse
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        viewLightDir = normalize(viewLightPos - ViewPosition);
        phongDiffuse += PhongDiffuse(viewNormal, PointLights[i].Intensity, viewLightDir);

        // specular
        blinnPhongSpecular += BlinnPhongSpecular(viewNormal, PointLights[i].Intensity, viewLightDir);
    }

    // directional lights
    for (uint i = 0; i < DirectionalLightsNumber; ++i)
    {
        // ambient
        phongAmbient += PhongAmbient(DirectionalLights[i].Intensity);
        
        viewLightDir = vec3(DirectionalLights[i].Direction[0], DirectionalLights[i].Direction[1], DirectionalLights[i].Direction[2]);
        viewLightDir = (NormalMatrix * vec4(viewLightDir, 1.0)).xyz;
        phongDiffuse += PhongDiffuse(viewNormal, DirectionalLights[i].Intensity, viewLightDir);

        // specular
        blinnPhongSpecular += BlinnPhongSpecular(viewNormal, DirectionalLights[i].Intensity, viewLightDir);
    }
    
    // create color
    vec3 color = Diffuse * phongAmbient + Diffuse * phongDiffuse + blinnPhongSpecular;

    return color;
}

layout (index = 5)
subroutine(ShadingType)
vec3 PhysicallyBasedShadingModel(vec3 viewNormal)
{
    vec3 outgoing = vec3(0.0);

    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 viewLightPos = vec3(0.0);
    vec3 viewLightDir = vec3(0.0);
    vec3 viewHalfDir = vec3(0.0);

    // point lights
    for (uint i = 0; i < PointLightsNumber; ++i)
    {
        diffuse = DiffuseBRDF();
        viewLightPos = (ViewMatrix * vec4(PointLights[i].Position[0], PointLights[i].Position[1], PointLights[i].Position[2], 1.0)).xyz;
        viewLightDir = normalize(viewLightPos - ViewPosition);
        viewHalfDir = normalize(viewLightDir + ViewDirection);
        specular = SpecularBRDF(viewNormal, viewLightDir, ViewDirection, viewHalfDir);
        outgoing += PointLights[i].Intensity * (diffuse + specular) * max(dot(viewNormal, viewLightDir), 0.0);
    }

    // directional lights
    for (uint i = 0; i < DirectionalLightsNumber; ++i)
    {
        diffuse = DiffuseBRDF();
        viewLightDir = vec3(DirectionalLights[i].Direction[0], DirectionalLights[i].Direction[1], DirectionalLights[i].Direction[2]);
        viewLightDir = (NormalMatrix * vec4(viewLightDir, 1.0)).xyz;
        viewHalfDir = normalize(viewLightDir + ViewDirection);
        specular = SpecularBRDF(viewNormal, viewLightDir, ViewDirection, viewHalfDir);
        outgoing += DirectionalLights[i].Intensity * (diffuse + specular) * max(dot(viewNormal, viewLightDir), 0.0);
    }

    return outgoing * pi;
}

// subroutines

void main()
{
    vec3 viewNormal = CurrentShadingMode();
    vec3 shading = CurrentShadingType(viewNormal);

    // gamma correction
    shading = shading / (shading + vec3(1.0));
    shading = pow(shading, vec3(1.0/2.2));  

    color = vec4(shading, 1.0);
}
