
#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{
    class TransformQuat
    {
    public:

        // statics
        static const glm::vec3 GlobalFront; // global front direction is positive x axis
        static const glm::vec3 GlobalUp; // global up direction is positive y axis
        static const glm::vec3 GlobalRight; // global right direction is positive z axis

        // @brief
        // Create a default quaternion in (0,0,0) position and pointing towards global front axis
        TransformQuat();

        // @brief
        // Rotate the transform by delta pitch, yaw and roll angles (in degrees)
        void Rotate(float deltaPitch, float deltaYaw, float deltaRoll);

        // getters

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

    protected:

        // @brief
        // Update rotation quaternion basing on Pitch, Yaw and Roll angles
        void UpdateRotation();

        // @brief
        // Update euler angles basing on pitch, yaw and roll
        void UpdateEulerAngles();

        glm::vec3 Position;
        glm::quat Rotation;

        // euler angles
        float Pitch; // around X axis
        float Yaw; // around Y axis
        float Roll; // around Z axis
    };
}
