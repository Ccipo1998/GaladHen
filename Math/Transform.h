
#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{
    class Transform
    {
    public:

        // statics
        static const glm::vec3 GlobalFront; // global front direction is positive x axis
        static const glm::vec3 GlobalUp; // global up direction is positive y axis
        static const glm::vec3 GlobalRight; // global right direction is positive z axis

        // @brief
        // Create a default quaternion in (0,0,0) position and pointing towards global front axis
        Transform();

        // @brief
        // Rotate the transform by delta pitch, yaw and roll angles (in degrees)
        void Rotate(float deltaPitch, float deltaYaw, float deltaRoll);

        // @brief
        // Rotate current orientation with a rotation defined in the global reference system (rotation * Orientation)
        void RotateGlobal(const glm::quat& rotation);

        // @brief
        // Rotate current orientation with a rotation defined in the local reference system (Orientation * rotation)
        void RotateLocal(const glm::quat& rotation);

        void RotatePitch(float deltaPitch);
        void RotateYaw(float deltaYaw);
        void RotateRoll(float deltaRoll);

        // getters

        glm::vec3 GetFront() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetRight() const;

        glm::vec3 GetPosition() const;
        glm::quat GetOrientation() const;

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
        void SetOrientation(const glm::quat& orientation);

        // @brief
        // Set pitch angle
        void SetPitch(float angle);

        // @brief
        // Set yaw angle
        void SetYaw(float angle);

        // @brief
        // Set roll angle
        void SetRoll(float angle);

        void SetScale(const glm::vec3& scale);

        void ScaleX(float scaleX);
        void ScaleY(float scaleY);
        void ScaleZ(float scaleZ);

        glm::vec3 GetScale() const;

        glm::mat4 ToMatrix() const;

        Transform Inverse() const;

        void LookAt(const glm::vec3& position);

    protected:

        // @brief
        // Update euler angles basing on pitch, yaw and roll
        void UpdateEulerAngles();

        glm::vec3 Position;
        glm::quat Orientation;
        glm::vec3 Scale;

        // euler angles
        float Pitch; // around X axis
        float Yaw; // around Y axis
        float Roll; // around Z axis
    };
}
