
const float ShadowBias = 0.00005; // TODO: add dynamic shadow bias basing on angle between normal and lightdir

uniform sampler2D ShadowMap;

float ShadowTest(vec4 lightSpaceFragPos)
{
	// Perspective division -> opengl do that automatically to gl_Position, we need to do that manually
	// From clip space to NDC ([-1,1])
	vec3 projectedLightSpaceFragPos = lightSpaceFragPos.xyz / lightSpaceFragPos.w;

	// We want to sample shadow map, which is inside [0,1] uv coords, so from NDC to [0,1]
	projectedLightSpaceFragPos = projectedLightSpaceFragPos * 0.5 + 0.5;

	// Sample shadow map and check its depth value (shadow caster depth) to fragment actual depth in light space (z component)
	float shadowCasterDepth = texture(ShadowMap, projectedLightSpaceFragPos.xy).r;
	float fragmentLightSpaceDepth = projectedLightSpaceFragPos.z;

	if (fragmentLightSpaceDepth < 0.0 || fragmentLightSpaceDepth > 1.0)
	{
		// When fragment depth is not in [0,1] range, it is outside light view frustum (i.e. we force no shadow)
		return 0.0;
	}

	return fragmentLightSpaceDepth > shadowCasterDepth + ShadowBias ? 1.0 : 0.0;
}
