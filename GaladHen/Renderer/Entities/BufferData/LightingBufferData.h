
#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct LightingBufferData
	{
		int PointLightNumber; // 8 byte
		int DirLightNumber; // 8 byte
		// No padding needed because it is a fixed buffer of two single data
		// TODO: padding could be different basing on low level api, implement a mechanism to calculate final size and alignment needed basing on api (with reflection?)
	};
}
