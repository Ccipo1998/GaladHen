
// Structs for data formatted as glsl shaders request

#pragma once

#include <glm/glm.hpp>

struct CameraData
{
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 WCameraPosition;
};

struct TransformData
{
	glm::mat4 ModelMatrix;
	glm::mat4 NormalMatrix;
};

struct PointLightData
{
	glm::vec4 Color;
	glm::vec3 Position;
	float Intensity;
	float BulbSize;
	float Radius;
};

struct DirectionalLightData
{
	glm::vec4 Color;
	glm::vec3 Position;
	float Intensity;
	glm::vec3 Direction;
};
