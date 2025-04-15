
// OpenGL Renderer

#pragma once

#include <Systems/RenderingSystem/LayerAPI/IRendererAPI.h>

#include <Utils/IdList.hpp>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

struct GLFWwindow;
class ImGuiContext;

namespace GaladHen
{
	class Texture;
	class Mesh;
	class IBuffer;
	enum class TextureFormat;

	class RendererGL : public IRendererAPI
	{
	public:

		RendererGL();

		virtual void Init() override;

		virtual void InitUI() override;

		virtual unsigned int CreateRenderBuffer(unsigned int width, unsigned int height, TextureFormat format, bool enableDepth) override;

		virtual void ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor) override;

		virtual void BindRenderBuffer(unsigned int renderBufferID) override;

		virtual void UnbindActiveRenderBuffer() override;

		virtual void Draw(CommandBuffer<RenderCommand>& renderCommandBuffer) override;

		virtual void TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer) override;

		virtual bool Compile(CommandBuffer<CompileCommand>& compileCommandBuffer) override; // TODO: CompileResult instead of bool as return type

		virtual void EnableDepthTest(bool enable) override;

		virtual void EnableBackFaceCulling(bool enable) override;

		virtual unsigned int GetRenderBufferColorApiID(unsigned int renderBufferID) override;

		virtual void SetViewport(const glm::uvec2& position, const glm::uvec2& size) override;

		virtual void CreateRenderingWindow(const char* name, glm::uvec2 size) override;

		virtual void SwapWindowBuffers() override;

		virtual void CloseRenderingWindow() override;

		virtual void BeforeDrawUI() override;

		virtual void DrawUI() override;

		~RendererGL();

		//virtual void SetViewport(const glm::uvec2& position, const glm::uvec2& size) override;

		//virtual void SetRenderTargetSize(const glm::uvec2& size) override;

	protected:

		static GLenum RendererGL::PrimitiveTypes[3];
		struct MeshGL
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
		static GLenum TextureChannelsAssociations[61];
		// OpenGL pixel data type
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml (type parameter)
		static GLenum PixelDataTypeAssociations[19];
		static GLint WrappingAssociations[4];
		static GLint FilteringAssociations[6];
		static GLenum TextureUnits[32];
		enum class TextureAllocationType
		{
			Dynamic,
			Constant
		};
		struct TextureGL // TODO: This struct should contain only opengl-specific data. All other data exists on high level classes
		{
			GLuint TextureID;
			TextureAllocationType AllocationType;
			GLenum TextureFormat;
			GLenum TextureChannels;
			GLenum PixelDataType;
			GLint Wrapping;
			GLint Filtering;
			GLenum TextureUnit;
			unsigned int Levels; // Number of mipmaps, with 1 = no mipmaps (only base level texture)
		};

		struct RenderBufferGL
		{
			GLuint FrameBufferID;
			unsigned int ColorTextureID;
			unsigned int DepthStencilTextureID; // zero if it is not attached
		};

		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
		static GLenum BufferTypesAssociations[14];
		static GLenum BufferUsageAssociations[9];

		struct BufferGL
		{
			GLuint BufferID;
			GLenum Target;
			GLenum ResourceProgramInterface;

			// Cache of previous allocation size, to know whether to reallocate a dynamic buffer or not
			size_t BytesSize;
		};

		// OPENGL -----------------------------------------------------------------------------------------------------------------------------------------

		unsigned int CreateTexture(const Texture& texture, TextureAllocationType allocationType = TextureAllocationType::Constant);
		unsigned int CreateDepthStencilTexture(unsigned int width, unsigned int height);
		void FreeTexture(unsigned int textureID);
		void LoadTexture(unsigned int textureID, const Texture& texture, TextureAllocationType allocationType = TextureAllocationType::Constant);

		unsigned int CreateMesh(const Mesh& mesh);
		void LoadMesh(unsigned int meshID, const Mesh& mesh);
		void FreeMesh(unsigned int meshID);

		unsigned int CreateBuffer(const IBuffer* buffer);
		void LoadBuffer(unsigned int bufferID, const IBuffer* buffer);
		void FreeBuffer(unsigned int bufferID);

		unsigned int CreateShaderPipeline(CompileCommand& compileCommand);
		bool CompileShaderPipeline(unsigned int shaderID, CompileCommand& compileCommand);
		bool CheckShaderPipelineCompilation(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		bool CheckShaderPipelineLinking(GLuint shaderProgram, char* outLog, unsigned int outLogLength);
		void FreeShaderPipeline(unsigned int shaderID);

		// UI
		void QuitUI();

		IdList<MeshGL> Meshes;
		IdList<BufferGL> Buffers;
		IdList<GLuint> Shaders;
		IdList<TextureGL> Textures;
		IdList<RenderBufferGL> RenderBuffers;

		GLFWwindow* Window;

		// UI
		ImGuiContext* ImGuiContext;

	};
}
