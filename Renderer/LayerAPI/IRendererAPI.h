
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
	class TransformQuat;

	class IRendererAPI
	{
	public:

		virtual void Init() = 0;

		virtual void LoadMeshData(Mesh& mesh) = 0;

		virtual void FreeMeshData(Mesh& mesh) = 0;

		virtual void LoadLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) = 0;

		virtual void UpdateLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) = 0;

		virtual void FreeLightingData() = 0;

		virtual CompilationResult CompileShaderPipeline(ShaderPipeline& pipeline) = 0;

		virtual CompilationResult CompileComputeShader(ComputeShader& compute) = 0;

		virtual void FreeShaderProgram(ShaderProgram* program) = 0;

		virtual void LoadTexture(Texture& texture) = 0;

		virtual void FreeTexture(Texture& texture) = 0;

		virtual void LoadMaterialData(Material& material) = 0;

		virtual void LoadCameraData(Camera& camera) = 0;

		virtual void UpdateCameraData(Camera& camera) = 0;

		virtual void LoadTransformData() = 0;

		virtual void UpdateTransformData(TransformQuat& transform) = 0;

		virtual void Draw(Mesh& mesh, Material& material) = 0;
	};
}
