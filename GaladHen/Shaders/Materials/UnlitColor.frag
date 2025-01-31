
#include "GaladHen/Shaders/ShadingModels/Unlit/Unlit.frag"

uniform vec4 ConstantColor = vec4(0.0);

vec4 ComputeUnlitColor()
{
	return ConstantColor;
}
