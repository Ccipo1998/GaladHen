
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

#include <vector>
#include <string>

#include <glm/fwd.hpp>

#include <Systems/RenderingSystem/CommandBuffer.h>

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
	class RenderBuffer;
	enum class TextureFormat;

	class IRendererAPI
	{
	public:

		virtual void Init() = 0;

		virtual void InitUI() = 0;

		virtual unsigned int CreateRenderBuffer(unsigned int width, unsigned int height, TextureFormat format, bool enableDepth) = 0;

		virtual void ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor) = 0;

		virtual void BindRenderBuffer(unsigned int renderBufferID) = 0;

		virtual void UnbindActiveRenderBuffer() = 0;

		virtual void Draw(CommandBuffer<RenderCommand>& renderCommandBuffer) = 0;

		// This function should write on each MemoryTransferCommand the new id of eventually created resource, in MemoryTargetID field
		virtual void TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer) = 0;

		virtual bool Compile(CommandBuffer<CompileCommand>& compileCommandBuffer) = 0; // TODO: CompileResult instead of bool as return type

		virtual void EnableDepthTest(bool enable) = 0;

		virtual void EnableBackFaceCulling(bool enable) = 0;

		virtual unsigned int GetRenderBufferColorApiID(unsigned int renderBufferID) = 0; // Probably it shouldn't exist, but for now i need it for ImGui usage

		virtual void SetViewport(const glm::uvec2& position, const glm::uvec2& size) = 0;

		// WINDOW FUNCTIONALITIES ------------------------------------------------------------------------------------------------------------------

		virtual void CreateRenderingWindow(const char* name, glm::uvec2 size) = 0;

		virtual void SwapWindowBuffers() = 0;

		virtual void CloseRenderingWindow() = 0;

		virtual void BeforeDrawUI() = 0;

		virtual void DrawUI() = 0;

		virtual ~IRendererAPI() {};
	};
}
