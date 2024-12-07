
// Datas that can be sent to shader pipelines

#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace GaladHen
{
	struct ShaderData
	{
		std::string DataName;
	};

	struct ShaderScalarData : public ShaderData
	{
		float Scalar;
	};

	struct ShaderVec2Data : public ShaderData
	{
		glm::vec2 Vec2;
	};

	struct ShaderVec3Data : public ShaderData
	{
		glm::vec3 Vec3;
	};

	struct ShaderVec4Data : public ShaderData
	{
		glm::vec4 Vec4;
	};

	struct ShaderIntegerData : public ShaderData
	{
		int32_t Integer;
	};

	struct ShaderTextureData : public ShaderData
	{
		unsigned int TextureID;
		unsigned int TextureUnit;
	};

	struct ShaderBufferData : ShaderData
	{
		unsigned int BufferID;
		unsigned int BufferBinding;
	};

	struct ShaderDataCollection
	{
		std::vector<ShaderScalarData> ScalarData;
		std::vector<ShaderVec2Data> Vec2Data;
		std::vector<ShaderVec3Data> Vec3Data;
		std::vector<ShaderVec4Data> Vec4Data;
		std::vector<ShaderIntegerData> IntegerData;
		std::vector<ShaderTextureData> TextureData;
		std::vector<ShaderData> FunctionsData;
		std::vector<ShaderBufferData> BufferData;
	};
}
