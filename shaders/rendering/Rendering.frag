
// Fragment shader for rendering.
// This shader offers many types of rendering

#version 450 core

// fragment color as output
layout (location = 0) out vec4 color;

// inputs
in vec3 ViewNormal;
in vec3 ViewLightDir;
in vec3 ViewDirection;

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
// light
uniform vec3 LightColor;
uniform float LightIntensity;

// functions

vec3 PhongAmbient()
{
    return Ka * LightIntensity;
}

vec3 PhongDiffuse()
{
    // diffuse reflection equation
    return Kd * LightIntensity * max(dot(ViewNormal, ViewLightDir), 0.0); // WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular
}

vec3 PhongSpecular()
{
    vec3 specularDir = normalize(-ViewLightDir + 2 * max(dot(ViewLightDir, ViewNormal), 0.0) * ViewNormal);
    return Ks * LightIntensity * pow(max(dot(specularDir, ViewDirection), 0.0), SpecularFalloff);
}

// subroutines to run current selected shading type
subroutine vec3 ShadingType();
subroutine uniform ShadingType CurrentShadingType;

subroutine(ShadingType)
vec3 PhongDiffuseReflection()
{
    // diffuse reflection equation
    vec3 diffuseReflection = Kd * LightIntensity * max(dot(ViewNormal, ViewLightDir), 0.0);

    // add color
    vec3 color = (Diffuse + LightColor) * diffuseReflection;

    return color;
}

subroutine(ShadingType)
vec3 PhongShadingModel()
{
    // add ambient, diffuse and specular
    vec3 ads = PhongAmbient() + PhongDiffuse() + PhongSpecular();

    // add color
    vec3 color = (Diffuse + LightColor) * ads;

    return color;
}

void main()
{
    vec3 diffuseColor = CurrentShadingType();
    color = vec4(diffuseColor, 1.0);
}
