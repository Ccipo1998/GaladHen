
#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct CameraBufferData
	{
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::vec3 CameraPosition;
	};
}
