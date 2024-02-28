
#pragma once

#define GLOBAL_FRONT glm::vec3(0.0f, 0.0f, -1.0f) // default front direction is negative z (because of OpenGL standard)
#define GLOBAL_UP glm::vec3(0.0f, 1.0f, 0.0f) // default up direction is positive y (because of OpenGL standard)
#define GLOBAL_RIGHT glm::vec3(1.0f, 0.0f, 0.0f) // default right direction is positive x (because of OpenGL standard)

#include <glm.hpp>
#include <ext/quaternion_float.hpp>

class TransformQuat
{
public:

    // @brief
    // Default constructor
    TransformQuat();

    // @brief
    // Get front vector
    glm::vec3 GetFront();

    // @brief
    // Get up vector
    glm::vec3 GetUp();

    // @brief
    // Get right vector
    glm::vec3 GetRight();

    glm::vec3 Position;
    glm::quat Rotation;

    // TODO: add euler angles
};
