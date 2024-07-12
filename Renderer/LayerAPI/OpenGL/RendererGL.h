
// OpenGL Renderer

#pragma once

#include <Renderer/LayerAPI/IRendererAPI.h>
#include <Renderer/LayerAPI/OpenGL/MeshGL.h>

#include <unordered_map>

namespace GaladHen
{
	class RendererGL : public IRendererAPI
	{

	public:

		virtual void Init() override;

		virtual unsigned int CreateLowLevelMesh() override;

		virtual void DestroyLowLevelMesh(unsigned int meshID) override;

		virtual void LoadMeshDataIntoGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int meshID) override;

		virtual void EnableDepthTest(bool enable) override;

	protected:

		std::unordered_map<unsigned int, MeshGL> Meshes;

	};
}
