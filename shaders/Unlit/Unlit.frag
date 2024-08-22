
// Unlit (solid color) fragment shader

#version 450 core

// outputs
out vec4 color;

// inputs
in VS_OUT
{
    in vec2 TexCoord;
} vs_out;

// const
const float gamma = 2.2;

// material data
uniform vec4 Diffuse;
uniform sampler2D DiffuseTexture;

// functions

vec3 GammaCorrection(vec3 shading)
{
    vec3 newShading = shading / (shading + vec3(1.0));
    newShading = pow(newShading, vec3(1.0/gamma));

    return newShading;
}

// subroutines

subroutine vec3 DiffuseMode();
layout (location = 1) subroutine uniform DiffuseMode GetDiffuse;

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

void main()
{
    // gamma correction
    vec3 solidColor = GetDiffuse();
    solidColor = GammaCorrection(solidColor);

    color = vec4(solidColor, 1.0);
}
