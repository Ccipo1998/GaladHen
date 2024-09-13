
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

	struct RayHitInfo
	{
		RayHitInfo()
			: HitDistance(std::numeric_limits<float>::max())
		{}

		bool Hit() { return HitDistance < std::numeric_limits<float>::max(); }

		float HitDistance;
	};

	struct RayTriangleHitInfo : RayHitInfo
	{
		RayTriangleHitInfo()
			: UV(glm::vec2(0.0f))
		{}

		glm::vec2 UV; // baricentric coordinates of intersection
	};

	struct RayTriangleMeshHitInfo : RayTriangleHitInfo
	{
		RayTriangleMeshHitInfo()
			: VertexIndex0(0)
			, VertexIndex1(0)
			, VertexIndex2(0)
		{}

		// Indices of the vertices array of the mesh, representing the primitive hitted
		unsigned int VertexIndex0;
		unsigned int VertexIndex1;
		unsigned int VertexIndex2;
	};

	struct RayModelHitInfo : RayTriangleMeshHitInfo
	{
		unsigned int MeshIndex;
	};
}
