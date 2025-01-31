
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
#include "GaladHen/Shaders/Common/GammaCorrection.glsl"

// Functions to define

vec4 ComputeUnlitColor();

void main()
{
    // gamma correction
    vec4 solidColor = ComputeUnlitColor();

    if (IsGammaCorrectionActive())
        solidColor = GammaCorrection(solidColor);

    color = vec4(solidColor);
}
