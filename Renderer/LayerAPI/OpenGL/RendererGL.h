
// OpenGL Renderer

#pragma once

#include <Renderer/LayerAPI/IRendererAPI.h>
#include <Renderer/LayerAPI/OpenGL/MeshGL.h>

#include <vector>

namespace GaladHen
{
	class RendererGL : public IRendererAPI
	{

	public:

		virtual void CreateLowLevelMesh() override;

		virtual void DestroyLowLevelMesh() override;

	protected:

		std::vector<MeshGL> Meshes;

	};
}
