
#pragma once

#include <string>

#include <Renderer/Common.h>

namespace GaladHen
{
	class ShaderPreprocessor
	{
	public:

		static std::string PreprocessShader(const std::string& shaderPath, API api);

	private:

		static std::string PreprocessShader_Recursive(const std::string& shaderPath, const std::string& shaderCode);

	};
}
