
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
in vec3 LightDir;
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

vec3 PhongDiffuse(vec3 viewNormal)
{
    // diffuse reflection equation
    return Kd * LightIntensity * max(dot(viewNormal, LightDir), 0.0); // WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular
}

vec3 PhongSpecular(vec3 viewNormal)
{
    vec3 specularDir = normalize(-LightDir + 2 * max(dot(LightDir, viewNormal), 0.0) * viewNormal);
    return Ks * LightIntensity * pow(max(dot(specularDir, ViewDirection), 0.0), SpecularFalloff);
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
    return Diffuse * PhongDiffuse(viewNormal);
}

layout (index = 3)
subroutine(ShadingType)
vec3 PhongShadingModel(vec3 viewNormal)
{
    // add ambient, diffuse and specular
    vec3 ambient = Diffuse * PhongAmbient();
    vec3 diffuse = Diffuse * PhongDiffuse(viewNormal);
    vec3 specular = LightColor * PhongSpecular(viewNormal); // specular doesnt take diffuse color because all the light is reflected in specular component -> it is not mixed with object color

    // create color
    vec3 color = ambient + diffuse + specular;

    return color;
}

// subroutines

void main()
{
    vec3 viewNormal = CurrentShadingMode();
    vec3 diffuseColor = CurrentShadingType(viewNormal);
    color = vec4(diffuseColor, 1.0);
}
