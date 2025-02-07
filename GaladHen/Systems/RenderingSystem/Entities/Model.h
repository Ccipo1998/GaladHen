
#pragma once

#include <vector>

#include "Mesh.h"

namespace GaladHen
{
	class Model
	{
		friend class BVH;

	public:

		Model(const std::vector<Mesh>& meshes);

		Model(const Model& source);
		Model& operator=(Model& source);
		Model(Model&& source) noexcept;
		Model& operator=(Model&& source) noexcept;

		std::vector<Mesh>& GetMeshes();

		BVH BVH;

	protected:

		std::vector<Mesh> Meshes;

	};
}
