
#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct DirLightBufferData
	{
		glm::vec4 Color; // 32 byte
		glm::vec3 Position; // 24 byte
		float Intensity; // 8 byte
		glm::vec3 Direction; // 24 byte
		float Padding; // 8 byte padding for structure alignment at multiple of biggest member size (Color, 32 byte)
		// TODO: padding could be different basing on low level api, implement a mechanism to calculate final size and alignment needed basing on api (with reflection?)
	};
}
