
#include "GaladHen/Shaders/ShadingModels/Pbr/Pbr.frag"

uniform sampler2D DiffuseTexture;
vec4 ComputeDiffuseColor()
{
	return texture(DiffuseTexture, vs_out.TexCoord);
}

uniform sampler2D NormalMap;
vec3 ComputeNormal()
{
	vec3 normalSample = texture(NormalMap, vs_out.TexCoord).rgb;
    normalSample = normalSample * 2.0 - 1.0;
    normalSample = normalize(vs_out.TBN * normalSample);
    return normalSample;
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
