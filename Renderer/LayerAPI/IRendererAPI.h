
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

#include <vector>
#include <string>

#include <Core/Light.h>

#include <glm/fwd.hpp>

#include <Renderer/CommandBuffer.h>

namespace GaladHen
{
	struct TextureData;

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

	class IRendererAPI
	{
	public:

		virtual void Init() = 0;

		virtual unsigned int CreateRenderBuffer(const TextureData& textureData) = 0;

		virtual void ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor) = 0;

		virtual void Draw(CommandBuffer<RenderCommand>& renderCommandBuffer) = 0;

		// This function should write on each MemoryTransferCommand the new id of eventually created resource, in MemoryTargetID field
		virtual void TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer) = 0;

		virtual bool Compile(CommandBuffer<CompileCommand&> compileCommandBuffer) = 0; // TODO: CompileResult instead of bool as return type

		virtual ~IRendererAPI() = 0;
	};
}
