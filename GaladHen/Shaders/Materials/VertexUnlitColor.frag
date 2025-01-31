
#include "GaladHen/Shaders/ShadingModels/Unlit/Unlit.frag"

vec4 ComputeUnlitColor()
{
	return vs_out.VertexColor;
}
