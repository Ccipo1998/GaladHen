
// OpenGL Renderer

#pragma once

#include <Renderer/LayerAPI/IRendererAPI.h>

#include <vector>

namespace GaladHen
{
	class MeshGL;

	class RendererGL : public IRendererAPI
	{

	public:

		virtual void CreateLowLevelMesh();

	protected:

		std::vector<MeshGL> Meshes;

	};
}
