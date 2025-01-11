
#include "ShaderPipeline.h"

namespace GaladHen
{
	ShaderPipeline::ShaderPipeline(const std::string vertexPath, const std::string tessContPath, const std::string tessEvalPath, const std::string geometryPath, const std::string fragmentPath)
		: VertexShaderPath(vertexPath)
		, TessContShaderPath(tessContPath)
		, TessEvalShaderPath(tessEvalPath)
		, GeometryShaderPath(geometryPath)
		, FragmentShaderPath(fragmentPath)
		, Type(ShaderPipelineType::ShaderPipeline)
	{}

	ShaderPipeline::ShaderPipeline(const std::string computePath)
		: ComputeShaderPath(computePath)
		, Type(ShaderPipelineType::ComputeShader)
	{}

	std::string ShaderPipeline::GetVertexShaderPath()
	{
		return VertexShaderPath;
	}

	std::string ShaderPipeline::GetTessContShaderPath()
	{
		return TessContShaderPath;
	}

	std::string ShaderPipeline::GetTessEvalShaderPath()
	{
		return TessEvalShaderPath;
	}

	std::string ShaderPipeline::GetGeometryShaderPath()
	{
		return GeometryShaderPath;
	}

	std::string ShaderPipeline::GetFragmentShaderPath()
	{
		return FragmentShaderPath;
	}

	std::string ShaderPipeline::GetComputeShaderPath()
	{
		return ComputeShaderPath;
	}

	ShaderPipelineType ShaderPipeline::GetType()
	{
		return Type;
	}
}
