
// OpenGL Renderer

#pragma once

#include "ShaderData.h"

#include <Renderer/LayerAPI/IRendererAPI.h>
#include <Renderer/LayerAPI/OpenGL/MeshGL.h>
#include <Renderer/LayerAPI/OpenGL/ShaderProgramGL.h>
#include <Renderer/LayerAPI/OpenGL/TextureGL.h>
#include <Renderer/LayerAPI/OpenGL/RenderBufferGL.h>

#include <Utils/IdList.hpp>

namespace GaladHen
{
	class RendererGL : public IRendererAPI
	{

	public:

		RendererGL();

		virtual void Init() override;

		virtual void BeginDraw() override;

		virtual void EndDraw() override;

		virtual void LoadMeshData(Mesh& mesh) override;

		virtual void FreeMeshData(Mesh& mesh) override;

		virtual void LoadLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) override;

		virtual void UpdateLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights) override;

		virtual void FreeLightingData() override;

		virtual CompilationResult CompileShaderPipeline(ShaderPipeline& pipeline) override;

		virtual CompilationResult CompileComputeShader(ComputeShader& compute) override;

		virtual void FreeShaderProgram(ShaderProgram* program) override;

		virtual void LoadTexture(Texture& texture) override;
		
		virtual void FreeTexture(Texture& texture) override;

		virtual void LoadMaterialData(Material& material) override;

		virtual void LoadCameraData(Camera& camera) override;

		virtual void UpdateCameraData(Camera& camera) override;

		virtual void LoadTransformData() override;

		virtual void UpdateTransformData(TransformQuat& transform) override;

		virtual void Draw(Mesh& mesh, Material& material) override;

		virtual IRenderBufferAPI* GetRenderBuffer() override;

		// OPENGL -----------------------------------------------------------------------------------------------------------------------------------------

		// @brief
		// Allocate a new buffer in gpu memory
		// @param bufferType: type of buffer to allocate
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param totalSizeBytes: the total occupancy in bytes of the buffer
		// @param data: pointer to data to load
		// @returns buffer id, to retrieve buffer for later use
		unsigned int LoadBufferData(GLenum bufferType, GLuint binding, GLenum usage, size_t totalSizeBytes, void* data);

		// @brief
		// Allocate a new buffer in gpu memory
		// // @param bufferType: type of buffer to allocate
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param sizesBytes: vector of sizes, each one referring to the pointer to data at the same index in data param
		// @param data: vector of pointers to data, each one with a total occupancy at the same index in sizesBytes param
		// @returns buffer id, to retrieve buffer for later use
		unsigned int LoadBufferData(GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& data);

		// @brief
		// Allocate a new buffer in gpu memory, overwriting the old allocation
		// @param bufferID: the id of the buffer to overwrite
		// @param bufferType: type of buffer to allocate
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param totalSizeBytes: the total occupancy in bytes of the buffer
		// @param data: pointer to data to load
		void LoadBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, size_t totalSizeBytes, void* data);

		// @brief
		// Allocate a new buffer in gpu memory, overwriting the old allocation
		// @param bufferID: the id of the buffer to overwrite
		// @param bufferType: type of buffer to allocate
		// @param binding: binding index inside the shader
		// @param usage: the usage of the buffer -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		// @param sizesBytes: vector of sizes, each one referring to the pointer to data at the same index in data param
		// @param data: vector of pointers to data, each one with a total occupancy at the same index in sizesBytes param
		void LoadBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& data);

		// @brief
		// Update data inside a buffer, without changing its size
		void UpdateBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, size_t offset, size_t totalSizeBytes, void* newData);

		// @brief
		// Update data inside a buffer, without changing its size
		void UpdateBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& newDatas);

		// @brief
		// Free memory allocated for a buffer
		void FreeBuffer(unsigned int bufferID);

		RenderBufferGL& GetFrontBuffer();

	protected:

		void DestroyLowLevelMesh(unsigned int meshID);
		void DestroyLowLevelShaderProgram(unsigned int shaderID);
		void DestroyLowLevelTexture(unsigned int textureID);

		// @brief
		// @returns buffer id, to retrieve buffer for later use
		unsigned int CreateBuffer();
		void DestroyBuffer(unsigned int bufferID);

		void TranslateToShaderData(const std::vector<PointLight>& pointLights, std::vector<PointLightData>& outLightData);
		void TranslateToShaderData(const std::vector<DirectionalLight>& dirLights, std::vector<DirectionalLightData>& outLightData);
		CameraData TranslateToShaderData(const Camera& camera);
		TransformData TranslateToShaderData(const TransformQuat& transform);

		void SwapFrontBuffer();
		void ClearFrontBuffer(bool colorBuffer, bool depthBuffer, bool stencilBuffer);

		IdList<MeshGL> Meshes;
		IdList<GLuint> Buffers;
		IdList<ShaderProgramGL> Shaders;
		IdList<TextureGL> Textures;

		// const data
		unsigned int PointLightBufferID;
		unsigned int DirectionalLightBufferID;
		unsigned int CameraDataUniformBufferID;
		unsigned int TransformDataUniformBufferID;

		// render targets for double buffering
		RenderBufferGL RenderBuffers[2];
		int32_t FrontRenderBufferIndex;

	};
}
