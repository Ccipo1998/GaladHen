
// Data about a buffer in gpu memory

// OpenGL: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml

#pragma once

#include <vector>

namespace GaladHen
{
	enum class BufferType
	{
		Uniform,
		ShaderStorage
	};

	enum class BufferAccessType
	{
		StaticRead,
		StaticWrite
	};

	struct BufferData
	{
		BufferType Type;
		BufferAccessType AccessType;
		std::vector<size_t> Sizes;
		std::vector<void*> Datas;
	};
}
