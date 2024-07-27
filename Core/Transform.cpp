
#include "Transform.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{
    const glm::vec3 TransformQuat::GlobalFront = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 TransformQuat::GlobalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 TransformQuat::GlobalRight = glm::vec3(0.0f, 0.0f, 1.0f);

    TransformQuat::TransformQuat()
        : Position(glm::vec3{})
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Pitch(0.0f)
        , Yaw(0.0f)
        , Roll(0.0f)
        {}

    void TransformQuat::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
    {
        Pitch += deltaPitch;
        Yaw += deltaYaw;
        Roll += deltaRoll;

        UpdateRotation();
    }

    glm::vec3 TransformQuat::GetFront()
    {
        return Rotation * GlobalFront;
    }

    glm::vec3 TransformQuat::GetUp()
    {
        return Rotation * GlobalUp;
    }

    glm::vec3 TransformQuat::GetRight()
    {
        return Rotation * GlobalRight;
    }

    glm::vec3 TransformQuat::GetPosition()
    {
        return Position;
    }

    glm::quat TransformQuat::GetRotation()
    {
        return Rotation;
    }

    float TransformQuat::GetPitch()
    {
        return Pitch;
    }

    float TransformQuat::GetYaw()
    {
        return Yaw;
    }

    float TransformQuat::GetRoll()
    {
        return Roll;
    }

    void TransformQuat::SetPosition(const glm::vec3& position)
    {
        Position = position;
    }

    void TransformQuat::SetRotation(const glm::quat& rotation)
    {
        Rotation = rotation;
        UpdateEulerAngles();
    }

    void TransformQuat::SetPitch(float angle)
    {
        Pitch = glm::mod(angle, 359.9999999f);
        UpdateRotation();
    }

    void TransformQuat::SetYaw(float angle)
    {
        Yaw = glm::mod(angle, 359.9999999f);
        UpdateRotation();
    }

    void TransformQuat::SetRoll(float angle)
    {
        Roll = glm::mod(angle, 359.9999999f);
        UpdateRotation();
    }

    // privates

    void TransformQuat::UpdateRotation()
    {
        float halfPitch = glm::radians(Pitch) * 0.5f;
        float sinPitch = glm::sin(halfPitch);
        float cosPitch = glm::cos(halfPitch);

        float halfYaw = glm::radians(Yaw) * 0.5f;
        float sinYaw = glm::sin(halfYaw);
        float cosYaw = glm::cos(halfYaw);

        float halfRoll = glm::radians(Roll) * 0.5f;
        float sinRoll = glm::sin(halfRoll);
        float cosRoll = glm::cos(halfRoll);

        Rotation.x = ((cosYaw * sinPitch) * cosRoll) + ((sinYaw * cosPitch) * sinRoll);
        Rotation.y = ((sinYaw * cosPitch) * cosRoll) - ((cosYaw * sinPitch) * sinRoll);
        Rotation.z = ((cosYaw * cosPitch) * sinRoll) - ((sinYaw * sinPitch) * cosRoll);
        Rotation.w = ((cosYaw * cosPitch) * cosRoll) + ((sinYaw * sinPitch) * sinRoll);
    }

    void TransformQuat::UpdateEulerAngles()
    {
        glm::vec3 eulers = glm::eulerAngles(Rotation);
        Pitch = eulers.x;
        Yaw = eulers.y;
        Roll = eulers.z;
    }
}
