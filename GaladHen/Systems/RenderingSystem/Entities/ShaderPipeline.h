
#pragma once

#include <string>

#include "IGPUResource.h"

namespace GaladHen
{
	enum class ShaderPipelineType
	{
		ShaderPipeline,
		ComputeShader
	};

	class ShaderPipeline : public IGPUResource
	{
	public:

		ShaderPipeline(const std::string vertexPath, const std::string tessContPath, const std::string tessEvalPath, const std::string geometryPath, const std::string fragmentPath);

		ShaderPipeline(const std::string computePath);

		ShaderPipeline(const ShaderPipeline& source) = delete;
		ShaderPipeline& operator=(ShaderPipeline& source) = delete;
		ShaderPipeline(ShaderPipeline&& source) = delete;
		ShaderPipeline& operator=(ShaderPipeline&& source) = delete;

		std::string GetVertexShaderPath() const;
		std::string GetTessContShaderPath() const;
		std::string GetTessEvalShaderPath() const;
		std::string GetGeometryShaderPath() const;
		std::string GetFragmentShaderPath() const;
		std::string GetComputeShaderPath() const;

		ShaderPipelineType GetType() const;

	protected:

		std::string VertexShaderPath;
		std::string TessContShaderPath;
		std::string TessEvalShaderPath;
		std::string GeometryShaderPath;
		std::string FragmentShaderPath;
		std::string ComputeShaderPath;

		ShaderPipelineType Type;
	};
}
