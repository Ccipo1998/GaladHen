
// All math operations needed by GaladHen

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct AABB;
	struct Ray;
	struct RayTriangleHitInfo;
	struct RayHitInfo;

	namespace Math
	{
		const float Epsilon = 0.0001f;

		// @brief
		// Calculate the centroid position of a triangle
		glm::vec3 TriangleCentroidPosition(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2);

		// @brief
		// Check if a ray intersects a triangle (Möller–Trumbore intersection algorithm)
		// @returns intersection info
		RayTriangleHitInfo RayTriangleIntersection(const Ray& ray, const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2);

		// @brief
		// Check if a ray intersects an axis aligned bounding box (slab test)
		// @returns intersection info
		RayHitInfo RayAABBIntersection(const Ray& ray, const AABB& aabb);
	}
}
