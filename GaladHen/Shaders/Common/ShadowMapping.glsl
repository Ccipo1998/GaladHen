
const float MinShadowBias = 0.000005;
const float MaxShadowBias = 0.00005;

const int ShadowSamples = 3;

uniform sampler2D ShadowMap;

float ShadowTest(vec4 lightSpaceFragPos, vec3 wNormal, vec3 wLightDir)
{
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(ShadowMap, 0); // the texel size is (1 / texture resolution) because uv mapping is in [0,1], so the division give us texel size

	// Perspective division -> opengl do that automatically to gl_Position, we need to do that manually
	// From clip space to NDC ([-1,1])
	vec3 projectedLightSpaceFragPos = lightSpaceFragPos.xyz / lightSpaceFragPos.w;

	// We want to sample shadow map, which is inside [0,1] uv coords, so from NDC to [0,1]
	projectedLightSpaceFragPos = projectedLightSpaceFragPos * 0.5 + 0.5;
	
	// Fragment actual depth in light space (z component)
	float fragmentLightSpaceDepth = projectedLightSpaceFragPos.z;

	if (fragmentLightSpaceDepth < 0.0 || fragmentLightSpaceDepth > 1.0)
	{
		// When fragment depth is not in [0,1] range, it is outside light view frustum (i.e. we force no shadow)
		return 0.0;
	}

	float shadowBias = max(MaxShadowBias * (1.0 - dot(wNormal, wLightDir)), MinShadowBias);

	// Apply PCF by sampling around the original sample point
	int sampleRange = ShadowSamples / 2;
	for (int x = -sampleRange; x <= sampleRange; ++x)
	{
		for (int y = -sampleRange; y <= sampleRange; ++y)
		{
			// Sample shadow map and check its depth value (shadow caster depth) to fragment actual depth in light space (z component)
			float shadowSampleDepth = texture(ShadowMap, projectedLightSpaceFragPos.xy + vec2(x, y) * texelSize).r;
			if (fragmentLightSpaceDepth > shadowSampleDepth + shadowBias)
			{
				shadow += 1.0;
			}
		}
	}

	// Average shadow value over number of samples
	float totalNumberOfSamples = pow(ShadowSamples, 2);
	return shadow / totalNumberOfSamples;
}
