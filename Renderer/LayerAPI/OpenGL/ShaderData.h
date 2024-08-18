
// Structs for data formatted as glsl shaders request

#pragma once

#include <glm/glm.hpp>

struct CameraData
{
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 NormalMatrix;
	glm::vec3 WCameraNormal;
};

struct PointLightData
{
	glm::vec4 Color;
	glm::vec3 Position;
	float Intensity;
	float FallOffDistance;
};

struct DirectionalLightData
{
	glm::vec4 Color;
	glm::vec3 Position;
	float Intensity;
	glm::vec3 Direction;
};
