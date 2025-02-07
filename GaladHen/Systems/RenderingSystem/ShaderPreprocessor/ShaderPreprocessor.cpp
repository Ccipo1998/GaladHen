
#include "ShaderPreprocessor.h"

#include <Utils/FileLoader.h>
#include <sstream>
#include <regex>
#include <Utils/Log.h>

#define INCLUDE_ERROR_DESCRIPTION(Path) std::string{"Formatting error for shader '#include' directive in "} + Path

namespace GaladHen
{
	std::string ShaderPreprocessor::PreprocessShader(const std::string& shaderPath, API api)
	{
		std::string shaderCode = FileLoader::ReadTextFile(shaderPath);

		if (shaderCode.size() == 0)
			return shaderCode;

		std::string outCode, shaderVersion;

		switch (api)
		{
		case API::OpenGL:

			outCode = shaderCode;
			shaderVersion = GH_GLSL_VERSION;
			shaderVersion += "\n\n";
			outCode.insert(0, shaderVersion);

			return PreprocessShader_Recursive(shaderPath, outCode);

			break;
		default:
			break;
		}
	}

	std::string ShaderPreprocessor::PreprocessShader_Recursive(const std::string& shaderPath, const std::string& shaderCode)
	{
		// Always same regex
		static std::regex regex{ R"(#include)" };
		static std::smatch smatch;

		// Iterate over lines to find an #include

		std::string outCode;
		bool includeFound = false;

		std::istringstream iss{ shaderCode };
		unsigned int track = 0;
		for (std::string line; std::getline(iss, line); )
		{
			if (std::regex_search(line, smatch, regex))
			{
				size_t start, end = -1;
				start = line.find('"');
				if (start != std::string::npos)
				{
					line.erase(0, start + 1);
				}
				else
				{
					// Formatting error
					Log::Error("Preprocessor", INCLUDE_ERROR_DESCRIPTION(shaderPath));

					return std::string{};
				}

				end = line.find('"');
				if (end != std::string::npos)
				{
					line.erase(end, end + line.size());
				}
				else
				{
					// Formatting error
					Log::Error("Preprocessor", INCLUDE_ERROR_DESCRIPTION(shaderPath));

					return std::string{};
				}

				outCode += FileLoader::ReadTextFile(line) + "\n";
				includeFound = true;
			}
			else
			{
				outCode += line + "\n";
			}
		}

		if (includeFound)
		{
			return PreprocessShader_Recursive(shaderPath, outCode);
		}

		return outCode;
	}
}
