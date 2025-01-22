
// Unlit (solid color) fragment shader

// outputs
out vec4 color;

// inputs
in VS_OUT
{
    in vec2 TexCoord;
    in vec4 VertexColor;
} vs_out;

// Gamma correction
#include "../GaladHen/Shaders/Common/GammaCorrection.glsl"

// functions

vec3 ComputeUnlitColor();

void main()
{
    // gamma correction
    vec3 solidColor = ComputeUnlitColor();

    if (IsGammaCorrectionActive())
        solidColor = GammaCorrection(solidColor);

    color = vec4(solidColor, 1.0);
}
