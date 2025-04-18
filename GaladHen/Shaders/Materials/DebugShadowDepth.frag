
#include "GaladHen/Shaders/ShadingModels/Unlit/Unlit.frag"

uniform sampler2D ShadowMap;

vec4 ComputeUnlitColor()
{
	float depthValue = texture(ShadowMap, vs_out.TexCoord).r;

	return vec4(vec3(depthValue), 1.0);
}

