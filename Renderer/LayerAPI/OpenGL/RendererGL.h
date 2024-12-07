
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
	struct MeshData;
	struct BufferData;
	struct TextureData;

	class RendererGL : public IRendererAPI
	{
	public:

		RendererGL();

		virtual void Init() override;

		virtual unsigned int CreateRenderBuffer(const TextureData& textureData) override;

		virtual void ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor) override;

		virtual void Draw(CommandBuffer<RenderCommand>& renderCommandBuffer) override;

		virtual void TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer) override;

		virtual bool Compile(CommandBuffer<CompileCommand&> compileCommandBuffer) override; // TODO: CompileResult instead of bool as return type

		virtual ~RendererGL() override;

		//virtual void SetViewport(const glm::uvec2& position, const glm::uvec2& size) override;

		//virtual void SetRenderTargetSize(const glm::uvec2& size) override;

	protected:

		static GLenum MeshGL::PrimitiveTypes[3];
		struct MeshGLTest
		{
			GLuint VAO, VBO, EBO;
			unsigned int NumberOfIndices;
			GLenum PrimitiveType;
		};

		// OpenGL texture formats are called Sized Internal Formats
		// https://www.khronos.org/opengl/wiki/Image_Format
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
		static GLenum TextureFormatAssociations[61];
		// OpenGL pixel formats are called Base Internal Formats
		// https://www.khronos.org/opengl/wiki/Image_Format
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
		static GLenum PixelChannelsAssociations[4];
		// OpenGL pixel data type
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml (type parameter)
		static GLenum PixelChannelDepthAssociations[19];
		static GLint WrappingAssociations[4];
		static GLint FilteringAssociations[6];
		static GLenum TextureUnits[32];
		enum class TextureAllocationType
		{
			Mutable,
			Immutable
		};
		struct TextureGLTest // TODO: This struct should contain only opengl-specific data. All other data exists on high level classes
		{
			GLuint TextureID;
			TextureAllocationType AllocationType;
			GLenum TextureFormat;
			GLenum PixelChannels;
			GLenum PixelChannelDepth;
			GLint Wrapping;
			GLint Filtering;
			GLenum TextureUnit;
			unsigned int NumberOfMipMaps;
		};

		struct RenderBufferGLTest
		{
			GLuint FrameBufferID;
			unsigned int ColorTextureID;
			unsigned int DepthStencilTextureID;
		};

		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		static GLenum BufferTypesAssociations[14];
		static GLenum BufferUsageAssociations[9];

		struct BufferGLTest
		{
			GLuint BufferID;
			GLenum Target;
		};

		// OPENGL -----------------------------------------------------------------------------------------------------------------------------------------

		unsigned int CreateTexture(const TextureData& textureData);
		unsigned int CreateDepthStencilTexture(unsigned int width, unsigned int height);
		void SetTextureParameters(const TextureGLTest& texture, const TextureParameters& params);
		void FreeTexture(unsigned int textureID);
		void LoadTexture(const TextureGLTest& texture, const TextureData& textureData);

		unsigned int CreateMesh(const MeshData& meshData);
		void LoadMesh(MeshGLTest& mesh, const MeshData& meshData);
		void FreeMesh(unsigned int meshID);

		unsigned int CreateBuffer(const BufferData& bufferData);
		void LoadBuffer(unsigned int bufferID, const BufferData& bufferData);
		void FreeBuffer(unsigned int bufferID);

		unsigned int CreateShaderPipeline(CompileCommand& compileCommand);
		bool CompileShaderPipeline(unsigned int shaderID, CompileCommand& compileCommand);
		bool CheckShaderPipelineCompilation(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		bool CheckShaderPipelineLinking(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		void FreeShaderPipeline(unsigned int shaderID);

		void TranslateToShaderData(const std::vector<PointLight>& pointLights, std::vector<PointLightData>& outLightData);
		void TranslateToShaderData(const std::vector<DirectionalLight>& dirLights, std::vector<DirectionalLightData>& outLightData);
		CameraData TranslateToShaderData(const Camera& camera);
		TransformData TranslateToShaderData(const TransformQuat& transform);

		IdList<MeshGLTest> Meshes;
		IdList<BufferGLTest> Buffers;
		IdList<GLuint> Shaders;
		IdList<TextureGLTest> Textures;
		IdList<RenderBufferGLTest> RenderBuffers;

	};
}
