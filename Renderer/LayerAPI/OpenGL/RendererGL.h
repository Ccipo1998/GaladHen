
// OpenGL Renderer

#pragma once

#include <Renderer/LayerAPI/IRendererAPI.h>
//#include <Renderer/LayerAPI/OpenGL/MeshGL.h>
//#include <Renderer/LayerAPI/OpenGL/ShaderProgramGL.h>
//#include <Renderer/LayerAPI/OpenGL/TextureGL.h>
//#include <Renderer/LayerAPI/OpenGL/RenderBufferGL.h>

#include <Utils/IdList.hpp>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

namespace GaladHen
{
	class Texture;
	class Mesh;
	class Buffer;

	class RendererGL : public IRendererAPI
	{
	public:

		RendererGL();

		virtual void Init() override;

		virtual unsigned int CreateRenderBuffer(unsigned int width, unsigned int height) override;

		virtual void ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor) override;

		virtual void Draw(CommandBuffer<RenderCommand>& renderCommandBuffer) override;

		virtual void TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer) override;

		virtual bool Compile(CommandBuffer<CompileCommand>& compileCommandBuffer) override; // TODO: CompileResult instead of bool as return type

		virtual void EnableDepthTest(bool enable) override;

		virtual unsigned int GetTextureApiID(unsigned int resourceID) override;

		~RendererGL();

		//virtual void SetViewport(const glm::uvec2& position, const glm::uvec2& size) override;

		//virtual void SetRenderTargetSize(const glm::uvec2& size) override;

	protected:

		static GLenum RendererGL::PrimitiveTypes[3];
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
			GLenum ResourceProgramInterface;
		};

		// OPENGL -----------------------------------------------------------------------------------------------------------------------------------------

		unsigned int CreateTexture(const Texture& texture);
		unsigned int CreateDepthStencilTexture(unsigned int width, unsigned int height);
		void FreeTexture(unsigned int textureID);
		void LoadTexture(unsigned int textureID, const Texture& texture);

		unsigned int CreateMesh(const Mesh& mesh);
		void LoadMesh(unsigned int meshID, const Mesh& mesh);
		void FreeMesh(unsigned int meshID);

		unsigned int CreateBuffer(const Buffer& buffer);
		void LoadBuffer(unsigned int bufferID, const Buffer& buffer);
		void FreeBuffer(unsigned int bufferID);

		unsigned int CreateShaderPipeline(CompileCommand& compileCommand);
		bool CompileShaderPipeline(unsigned int shaderID, CompileCommand& compileCommand);
		bool CheckShaderPipelineCompilation(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		bool CheckShaderPipelineLinking(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		void FreeShaderPipeline(unsigned int shaderID);

		IdList<MeshGLTest> Meshes;
		IdList<BufferGLTest> Buffers;
		IdList<GLuint> Shaders;
		IdList<TextureGLTest> Textures;
		IdList<RenderBufferGLTest> RenderBuffers;

	};
}
