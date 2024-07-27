
// OpenGL Renderer

#pragma once

#include <Renderer/LayerAPI/IRendererAPI.h>
#include <Renderer/LayerAPI/OpenGL/MeshGL.h>
#include <Renderer/LayerAPI/OpenGL/ShaderProgramGL.h>
#include <Renderer/LayerAPI/OpenGL/TextureGL.h>

#include <vector>

namespace GaladHen
{
	class RendererGL : public IRendererAPI
	{

	public:

		RendererGL();

		virtual void Init() override;

		virtual void LoadMeshDataIntoGPU(Mesh& mesh) override;

		virtual void FreeMeshDataFromGPU(Mesh& mesh) override;

		virtual void LoadLighingDataIntoGPU(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) override;

		virtual void FreeLightingDataFromGPU() override;

		virtual CompilationResult CompileShaderPipeline(ShaderPipeline& pipeline) override;

		virtual CompilationResult CompileComputeShader(ComputeShader& compute) override;

		virtual void FreeShaderProgram(ShaderProgram* program) override;

		virtual void LoadTextureIntoGPU(Texture& texture) override;
		
		virtual void FreeTextureFromGPU(Texture& texture) override;

		virtual void EnableDepthTest(bool enable) override;

		virtual void LoadMaterialData(Material& material) override;

		virtual void Draw(Mesh& mesh, Material& material) override;

		// OPENGL -----------------------------------------------------------------------------------------------------------------------------------------

		// @brief
		// Allocate a new buffer in gpu memory
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param totalSizeBytes: the total occupancy in bytes of the buffer
		// @param data: pointer to data to load
		// @returns buffer id, to retrieve buffer for later use
		unsigned int LoadBufferData(GLuint binding, GLenum usage, size_t totalSizeBytes, void* data);

		// @brief
		// Allocate a new buffer in gpu memory
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param sizesBytes: vector of sizes, each one referring to the pointer to data at the same index in data param
		// @param data: vector of pointers to data, each one with a total occupancy at the same index in sizesBytes param
		// @returns buffer id, to retrieve buffer for later use
		unsigned int LoadBufferData(GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*> data);

		// @brief
		// Update data inside a buffer, potentially changing its size
		void UpdateBufferData(unsigned int bufferID, GLuint binding, GLenum usage, size_t totalSizeBytes, void* newData);

		// @brief
		// Update data inside a buffer, potentially changing its size
		void UpdateBufferData(unsigned int bufferID, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*> data);

		// @brief
		// Free memory allocated for a buffer
		void FreeBufferData(unsigned int bufferID);

	protected:

		unsigned int CreateLowLevelMesh();
		void DestroyLowLevelMesh(unsigned int meshID);
		unsigned int CreateLowLevelShaderProgram();
		void DestroyLowLevelShaderProgram(unsigned int shaderID);
		unsigned int CreateLowLevelTexture();
		void DestroyLowLevelTexture(unsigned int textureID);

		std::vector<MeshGL> Meshes;
		unsigned int MeshIndex;

		std::vector<GLuint> Buffers;
		unsigned int BufferIndex;
		unsigned int LightingBufferID;

		std::vector<ShaderProgramGL> Shaders;
		unsigned int ShaderIndex;

		std::vector<TextureGL> Textures;
		unsigned int TextureIndex;

	};
}
