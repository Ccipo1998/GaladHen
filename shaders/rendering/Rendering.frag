
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
uniform float Kd;
// light
uniform vec3 LightColor;
uniform float LightIntensity;

// subroutines to run current selected shading type
subroutine vec3 ShadingType();
subroutine uniform ShadingType CurrentShadingType;

subroutine(ShadingType)
vec3 PhongDiffuseReflection()
{
    // diffuse reflection equation
    float diffuseReflection = Kd * LightIntensity * max(dot(ViewNormal, ViewLightDir), 0.0);

    // add color
    vec3 color = (Diffuse + LightColor) * diffuseReflection;

    return color;
}

void main()
{
    vec3 diffuseColor = CurrentShadingType();
    color = vec4(diffuseColor, 1.0);
}
