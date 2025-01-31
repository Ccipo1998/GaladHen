
#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct PointLightBufferData
	{
		glm::vec4 Color; // 32 byte
		glm::vec3 Position; // 24 byte
		float Intensity; // 8 byte
		float BulbSize; // 8 byte
		float Radius; // 8 byte
		glm::vec2 Padding; // 16 byte padding for structure alignment at multiple of biggest member size (Color, 32 byte)
		// TODO: padding could be different basing on low level api, implement a mechanism to calculate final size and alignment needed basing on api (with reflection?)
	};
}
