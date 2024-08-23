
// Ray data structures

#pragma once

#include <glm/glm.hpp>
#include <limits>

namespace GaladHen
{
	struct Ray
	{
		Ray()
			: Origin(glm::vec3(0.0f))
			, Direction(glm::vec3(0.0f))
			, Length(0.0f)
		{}

		Ray(const glm::vec3& origin, const glm::vec3& direction, float length)
			: Origin(origin)
			, Direction(glm::normalize(direction))
			, Length(length)
		{}

		glm::vec3 Origin;
		glm::vec3 Direction;
		float Length;
	};

	struct RayTriangleHitInfo
	{
		RayTriangleHitInfo()
			: HitDistance(std::numeric_limits<float>::max())
			, UV(glm::vec2(0.0f))
		{}

		bool Hit() { return HitDistance < std::numeric_limits<float>::max(); }

		float HitDistance;
		glm::vec2 UV; // baricentric coordinates of intersection
	};

	struct RayTriangleMeshHitInfo
	{
		bool Hit() { return RayTriangleHit.HitDistance < std::numeric_limits<float>::max(); }

		RayTriangleHitInfo RayTriangleHit;
		
		// Indices of the vertices array of the mesh, representing the primitive hitted
		unsigned int Index0;
		unsigned int Index1;
		unsigned int Index2;
	};
}
