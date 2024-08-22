
// All math operations needed by GaladHen

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	namespace Math
	{
		// @brief
		// Calculate the centroid position of a triangle
		glm::vec3 TriangleCentroidPosition(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2);
	}
}
