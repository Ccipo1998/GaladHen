
// A generic buffer of commands, used by the renderer

#pragma once

#include <vector>
#include <memory>
#include <string>

#include <Renderer/Entities/Material.h>

namespace GaladHen
{
	template <class T>
	class CommandBuffer : public std::vector<T>
	{
	public:


	};

	// A RenderCommand targets resource ids: must be already transferred into gpu, or the RenderCommand will fail
	struct RenderCommand
	{
		unsigned int DataSourceID;
		// unsigned int InstanceCount; TODO: instanced rendering
		unsigned int ShaderSourceID;
		std::shared_ptr<Material> Material;
		std::unordered_map<std::string, std::shared_ptr<IBuffer>> AdditionalBufferData; // Like buffers managed by renderer (camera data, transform data, ...)
	};

	enum class MemoryTargetType
	{
		Mesh,
		Buffer,
		Texture
	};

	enum class MemoryTransferType
	{
		Load,
		Free
	};

	// A MemoryTransferCommand is generic, it will be interpreted by api-level renderer
	struct MemoryTransferCommand
	{
		void* Data;
		unsigned int MemoryTargetID;
		MemoryTargetType TargetType;
		MemoryTransferType TransferType;
	};

	enum class CompileType
	{
		Compile,
		Free
	};

	struct CompileResult
	{
		bool Succeed;
		std::string Description;
	};

	struct CompileCommand
	{
		std::string VertexCode;
		std::string TessContCode;
		std::string TessEvalCode;
		std::string GeometryCode;
		std::string FragmentCode;
		std::string ComputeCode;
		unsigned int ShaderPipelineID;
		CompileType Type;
		CompileResult Result;
	};
}
