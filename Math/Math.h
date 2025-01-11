
// All math operations needed by GaladHen

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
	struct AABB;
	struct Ray;
	struct RayTriangleHitInfo;
	struct RayTriangleMeshHitInfo;
	struct RayHitInfo;
	struct RayModelHitInfo;
	class BVH;
	enum class BVHTraversalMethod;
	class Mesh;
	class Model;
	class Transform;

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

		// @brief
		// Check if a ray intersects an triangle mesh, using its BVH
		// @returns intersection info
		RayTriangleMeshHitInfo RayTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVH& bvh, BVHTraversalMethod traversalMethod);

		// @brief
		// Check if a ray intersects an triangle mesh with a transform applied, using its BVH
		// @returns intersection info
		RayTriangleMeshHitInfo RayTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVH& bvh, const Transform& transform, BVHTraversalMethod traversalMethod);

		// @brief
		// Check if a ray intersects a model (set of triangle meshes), using its BVH
		// @returns intersection info
		RayModelHitInfo RayModelIntersection(const Ray& ray, const Model& model, const BVH& bvh, BVHTraversalMethod traversalMethod);

		// @brief
		// Check if a ray intersects a model (set of triangle meshes) with a transform applied, using its BVH
		// @returns intersection info
		RayModelHitInfo RayModelIntersection(const Ray& ray, const Model& model, const BVH& bvh, const Transform& transform, BVHTraversalMethod traversalMethod);

		Ray operator*(const Transform transform, const Ray ray);
		Ray operator*(const Ray ray, const Transform transform);
	}
}
