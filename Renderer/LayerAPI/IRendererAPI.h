
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

#include <vector>
#include <string>

#include <GaladHen/MeshData.h>
#include <GaladHen/Light.h>

namespace GaladHen
{
	class CompilationResult;

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

		virtual void LoadLighingDataIntoGPU(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) = 0;

		virtual void FreeLightingDataFromGPU() = 0;

		// @brief
		// Create a low level shader program
		// @return The ID of the newly created low level shader program
		virtual unsigned int CreateLowLevelShaderProgram() = 0;

		// @brief
		// Destroy a low level shader program, freeing gpu memory
		virtual void DestroyLowLevelShaderProgram(unsigned int shaderID) = 0;

		virtual CompilationResult CompileShaderProgramPipeline(std::string& vertexCode, std::string& tessContCode, std::string& tessEvalCode, std::string& geometryCode, std::string& fragmentCode, unsigned int shaderID) = 0;

		virtual CompilationResult CompilerShaderProgram(std::string& computeCode, unsigned int shaderID) = 0;

		virtual void EnableDepthTest(bool enable) = 0;

	};
}
