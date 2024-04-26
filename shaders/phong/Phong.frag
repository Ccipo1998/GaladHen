
// Phong fragment shader
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
in vec2 TexCoord;

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

    return DiffuseColor * phongDiffuse;
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
    vec3 color = DiffuseColor * phongAmbient + DiffuseColor * phongDiffuse + phongSpecular;

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
    vec3 color = DiffuseColor * phongAmbient + DiffuseColor * phongDiffuse + blinnPhongSpecular;

    return color;
}

// subroutines

void main()
{
    // shading
    vec3 viewNormal = CurrentShadingMode();
    vec3 shading = CurrentShadingType(viewNormal);

    // gamma correction
    shading = shading / (shading + vec3(1.0));
    shading = pow(shading, vec3(1.0/2.2));  

    color = vec4(shading, 1.0);
}
