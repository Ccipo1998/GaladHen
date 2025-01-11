
#include "Math.h"

#include "Ray.h"
#include "AABB/AABB.h"
#include "BVH/BVH.h"
#include "Transform.h"

namespace GaladHen
{
	namespace Math
	{
		glm::vec3 TriangleCentroidPosition(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2)
		{
			return (v0 + v1 + v2) * 0.3333f;
		}

		RayTriangleHitInfo RayTriangleIntersection(const Ray& ray, const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2)
		{
			// Möller–Trumbore intersection algorithm (from "Fast, Minimum Storage Ray/Triangle Intersection" paper by Tomas Möller and Ben Trumbore)
			// https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

			RayTriangleHitInfo intersection{};

			// triangle edges
			const glm::vec3 e1 = v1 - v0;
			const glm::vec3 e2 = v2 - v0;

			// determinant calculation
			// - to check if the ray lies on the triangle plane (culling test)
			// - to calculate u
			const glm::vec3 h = glm::cross(ray.Direction, e2);
			const float a = glm::dot(e1, h); // determinant
			if (a > -Epsilon && a < Epsilon)
				return intersection; // ray parallel to triangle

			const float f = 1 / a; // inverse determinant
			const glm::vec3 s = ray.Origin - v0; // distance from vert0 to ray origin

			// calculating u for test bounds
			const float u = f * glm::dot(s, h);
			if (u < 0.0f || u > 1.0f)
				return intersection; // if u is outside the required range for the barycentric coordinate -> the interseciton point lies on the triangle plane but outside the triangle (culling test)

			// calculating u for test bounds
			const glm::vec3 q = glm::cross(s, e1);
			const float v = f * glm::dot(ray.Direction, q);
			if (v < 0.0f || u + v > 1.0f)
				return intersection; // if v is outside the required range for the barycentric coordinate -> the interseciton point lies on the triangle plane but outside the triangle (culling test)
			
			// in fact the requirements for barycentric coordinates are:
			// - u >= 0
			// - v >= 0
			// - u + v <= 1

			// here we have hitted the triangle surface -> we calculate the parameter t which gives the distance on the ray from its origin to the intersection point
			const float t = f * glm::dot(e2, q);
			if (t > Epsilon)
			{
				intersection.HitDistance = t;
				intersection.UV = glm::vec2(u, v);
			}

			return intersection;
		}

		RayHitInfo RayAABBIntersection(const Ray& ray, const AABB& aabb)
		{
			float tx1 = (aabb.MinBound.x - ray.Origin.x) / ray.Direction.x, tx2 = (aabb.MaxBound.x - ray.Origin.x) / ray.Direction.x;
			float tmin = glm::min(tx1, tx2), tmax = glm::max(tx1, tx2);
			float ty1 = (aabb.MinBound.y - ray.Origin.y) / ray.Direction.y, ty2 = (aabb.MaxBound.y - ray.Origin.y) / ray.Direction.y;
			tmin = glm::max(tmin, glm::min(ty1, ty2)), tmax = glm::min(tmax, glm::max(ty1, ty2));
			float tz1 = (aabb.MinBound.z - ray.Origin.z) / ray.Direction.z, tz2 = (aabb.MaxBound.z - ray.Origin.z) / ray.Direction.z;
			tmin = glm::max(tmin, glm::min(tz1, tz2)), tmax = glm::min(tmax, glm::max(tz1, tz2));

			if (tmax >= tmin && tmin < ray.Length && tmax > 0)
			{
				RayHitInfo info{};
				info.HitDistance = tmin;

				return info;
			}

			RayHitInfo info{};
			info.HitDistance = std::numeric_limits<float>::max();
		}

		RayTriangleMeshHitInfo RayTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVH& bvh, BVHTraversalMethod traversalMethod)
		{
			return bvh.CheckTriangleMeshIntersection(ray, mesh, traversalMethod);
		}

		RayTriangleMeshHitInfo RayTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVH& bvh, const Transform& transform, BVHTraversalMethod traversalMethod)
		{
			// Transform world space ray into given transform space
			Ray inverseRay = transform.Inverse() * ray;

			return bvh.CheckTriangleMeshIntersection(ray, mesh, traversalMethod);
		}

		RayModelHitInfo RayModelIntersection(const Ray& ray, const Model& model, const BVH& bvh, BVHTraversalMethod traversalMethod)
		{
			return bvh.CheckModelIntersection(ray, model, traversalMethod);
		}

		RayModelHitInfo RayModelIntersection(const Ray& ray, const Model& model, const BVH& bvh, const Transform& transform, BVHTraversalMethod traversalMethod)
		{
			// Transform world space ray into given transform space
			Ray inverseRay = transform.Inverse() * ray;

			return bvh.CheckModelIntersection(ray, model, traversalMethod);
		}

		Ray operator*(const Transform transform, const Ray ray)
		{
			return Ray{ transform.ToMatrix() * glm::vec4(ray.Origin, 1.0f), transform.GetOrientation() * glm::vec4(ray.Direction, 1.0f), ray.Length };
		}
		Ray operator*(const Ray ray, const Transform transform)
		{
			return Ray{ glm::vec4(ray.Origin, 1.0f) * transform.ToMatrix(), glm::vec4(ray.Direction, 1.0f) * transform.GetOrientation(), ray.Length };
		}
	}
}
