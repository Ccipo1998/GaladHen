
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

#include <vector>

#include <GaladHen/MeshData.h>

namespace GaladHen
{
	class IRendererAPI
	{
	public:

		// @brief
		// Init operations
		virtual void Init() = 0;

		// @brief
		// Create a low level mesh
		// @return The ID of the newly created low level mesh
		virtual unsigned int CreateLowLevelMesh() = 0;

		// @brief
		// Destroy a low level mesh, freeing gpu memory
		virtual void DestroyLowLevelMesh(unsigned int meshID) = 0;

		virtual void LoadMeshDataIntoGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int meshID) = 0;

		virtual void EnableDepthTest(bool enable) = 0;

	};
}
