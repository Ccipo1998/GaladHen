
#include "GaladHen/Shaders/ShadingModels/Unlit/Unlit.frag"

vec3 ComputeUnlitColor()
{
	return vs_out.VertexColor.rgb;
}
