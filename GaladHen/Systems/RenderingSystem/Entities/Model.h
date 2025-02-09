
#pragma once

#include <vector>

#include "Mesh.h"

namespace GaladHen
{
	class Model
	{
		friend class BVH;

	public:

		Model();
		Model(const std::vector<Mesh>& meshes);

		Model(const Model& source);
		Model& operator=(Model& source);
		Model(Model&& source) noexcept;
		Model& operator=(Model&& source) noexcept;

		BVH BVH;
		std::vector<Mesh> Meshes;

	};
}
