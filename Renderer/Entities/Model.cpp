
#include "Model.h"

namespace GaladHen
{
	Model::Model(const std::vector<Mesh>& meshes)
		: Meshes(meshes)
	{}

	Model::Model(const Model& source)
	{
		Meshes = source.Meshes; // triggers copy constructor of each mesh, which invalidate the new gpu resource
		BVH = source.BVH;
	}

	Model& Model::operator=(Model& source)
	{
		Meshes = source.Meshes; // triggers copy constructor of each mesh, which invalidate the new gpu resource
		BVH = source.BVH;

		return *this;
	}

	Model::Model(Model&& source) noexcept
	{
		Meshes = std::move(source.Meshes);
		BVH = std::move(source.BVH);
	}

	Model& Model::operator=(Model&& source) noexcept
	{
		Meshes = std::move(source.Meshes);
		BVH = std::move(source.BVH);

		return *this;
	}

	std::vector<Mesh>& Model::GetMeshes()
	{
		return Meshes;
	}
}
