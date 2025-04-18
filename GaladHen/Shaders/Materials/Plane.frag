
#include "GaladHen/Shaders/ShadingModels/Pbr/Pbr.frag"

uniform vec4 DiffuseConstant = vec4(1.0);
vec4 ComputeDiffuseColor()
{
	return DiffuseConstant;
}

vec3 ComputeNormal()
{
	return vs_out.SmoothWNormal;
}

uniform float Metallic = 0.0;
float ComputeMetallic()
{
    return Metallic;
}

uniform float Roughness = 1.0;
float ComputeRoughness()
{
    return Roughness;
}
