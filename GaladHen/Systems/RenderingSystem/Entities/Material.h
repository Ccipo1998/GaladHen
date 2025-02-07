
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Buffer.hpp"

namespace GaladHen
{
	class Texture;
	class ShaderPipeline;

	class Material
	{
	public:

		Material(std::shared_ptr<ShaderPipeline> pipeline);

		Material(const Material& source) = delete;
		Material& operator=(Material& source) = delete;
		Material(Material&& source) = delete;
		Material& operator=(Material&& source) = delete;

		std::shared_ptr<ShaderPipeline> GetPipeline();

		std::unordered_map<std::string, float> ScalarData;
		std::unordered_map<std::string, glm::vec2> Vec2Data;
		std::unordered_map<std::string, glm::vec3> Vec3Data;
		std::unordered_map<std::string, glm::vec4> Vec4Data;
		std::unordered_map<std::string, glm::mat3> Mat3Data;
		std::unordered_map<std::string, glm::mat4> Mat4Data;
		std::unordered_map<std::string, std::shared_ptr<Texture>> TextureData;
		std::unordered_map<std::string, std::shared_ptr<IBuffer>> BufferData;

	protected:

		std::shared_ptr<ShaderPipeline> Pipeline;

	};
}
