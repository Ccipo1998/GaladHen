
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

#include <vector>
#include <string>

#include <Core/Light.h>

namespace GaladHen
{
	class CompilationResult;
	class Material;
	class Mesh;
	class ShaderPipeline;
	class ComputeShader;
	class Texture;
	class ShaderProgram;
	class Camera;

	class IRendererAPI
	{
	public:

		virtual void Init() = 0;

		virtual void LoadMeshDataIntoGPU(Mesh& mesh) = 0;

		virtual void FreeMeshDataFromGPU(Mesh& mesh) = 0;

		virtual void LoadLighingDataIntoGPU(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) = 0;

		virtual void FreeLightingDataFromGPU() = 0;

		virtual CompilationResult CompileShaderPipeline(ShaderPipeline& pipeline) = 0;

		virtual CompilationResult CompileComputeShader(ComputeShader& compute) = 0;

		virtual void FreeShaderProgram(ShaderProgram* program) = 0;

		virtual void LoadTextureIntoGPU(Texture& texture) = 0;

		virtual void FreeTextureFromGPU(Texture& texture) = 0;

		virtual void EnableDepthTest(bool enable) = 0;

		virtual void LoadMaterialData(Material& material) = 0;

		virtual void LoadCameraData(Camera& camera) = 0;

		virtual void Draw(Mesh& mesh, Material& material) = 0;
	};
}
