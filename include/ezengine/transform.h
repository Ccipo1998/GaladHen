
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
    // Rotate the transform by delta pitch, yaw and roll angles (in degrees)
    void Rotate(float deltaPitch, float deltaYaw, float deltaRoll);

    // getters

    // @brief
    // Get global default front axis
    static glm::vec3 GetGlobalFront();

    // @brief
    // Get global default up axis
    static glm::vec3 GetGlobalUp();

    // @brief
    // Get global default right axis
    static glm::vec3 GetGlobalRight();

    glm::vec3 GetFront();
    glm::vec3 GetUp();
    glm::vec3 GetRight();

    glm::vec3 GetPosition();
    glm::quat GetRotation();

    // @brief
    // Get pitch angle
    float GetPitch();

    // @brief
    // Get yaw angle
    float GetYaw();

    // @brief
    // Get roll angle
    float GetRoll();

    // setters

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::quat& rotation);

    // @brief
    // Set pitch angle
    void SetPitch(float angle);

    // @brief
    // Set yaw angle
    void SetYaw(float angle);

    // @brief
    // Set roll angle
    void SetRoll(float angle);

private:

    glm::vec3 Position;
    glm::quat Rotation;

    // euler angles
    float Pitch; // around X axis
    float Yaw; // around Y axis
    float Roll; // around Z axis

    // @brief
    // Update rotation quaternion basing on Pitch, Yaw and Roll angles
    void UpdateRotation();

    // @brief
    // Update euler angles basing on pitch, yaw and roll
    void UpdateEulerAngles();
};
