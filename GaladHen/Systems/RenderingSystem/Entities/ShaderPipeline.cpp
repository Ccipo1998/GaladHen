
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

	std::string ShaderPipeline::GetVertexShaderPath() const
	{
		return VertexShaderPath;
	}

	std::string ShaderPipeline::GetTessContShaderPath() const
	{
		return TessContShaderPath;
	}

	std::string ShaderPipeline::GetTessEvalShaderPath() const
	{
		return TessEvalShaderPath;
	}

	std::string ShaderPipeline::GetGeometryShaderPath() const
	{
		return GeometryShaderPath;
	}

	std::string ShaderPipeline::GetFragmentShaderPath() const
	{
		return FragmentShaderPath;
	}

	std::string ShaderPipeline::GetComputeShaderPath() const
	{
		return ComputeShaderPath;
	}

	ShaderPipelineType ShaderPipeline::GetType() const
	{
		return Type;
	}
}
