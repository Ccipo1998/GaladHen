
#include "Math.h"

namespace GaladHen
{
	namespace Math
	{
		// @brief
		// Calculate the centroid position of a triangle
		glm::vec3 TriangleCentroidPosition(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
		{
			return (v0 + v1 + v2) * 0.3333f;
		}
	}
}
