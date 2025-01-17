
// Unlit (solid color) fragment shader

#version 450 core

// outputs
out vec4 color;

// inputs
in VS_OUT
{
    in vec2 TexCoord;
    in vec4 VertexColor;
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


void main()
{
    // gamma correction
    vec3 solidColor = vs_out.VertexColor.xyz;
    solidColor = GammaCorrection(solidColor);

    color = vec4(solidColor, 1.0);
}
