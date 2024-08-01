
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
        , Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Pitch(0.0f)
        , Yaw(0.0f)
        , Roll(0.0f)
        {}

    void TransformQuat::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
    {
        RotatePitch(deltaPitch);
        RotateYaw(deltaYaw);
        RotateRoll(deltaRoll);
    }

    void TransformQuat::RotatePitch(float deltaPitch)
    {
        glm::quat rotate = glm::angleAxis(glm::radians(deltaPitch), GlobalRight);
        Orientation *= rotate;

        UpdateEulerAngles();
    }

    void TransformQuat::RotateYaw(float deltaYaw)
    {
        glm::quat rotate = glm::angleAxis(glm::radians(deltaYaw), GlobalUp);
        Orientation *= rotate;

        UpdateEulerAngles();
    }

    void TransformQuat::RotateRoll(float deltaRoll)
    {
        glm::quat rotate = glm::angleAxis(glm::radians(deltaRoll), GlobalFront);
        Orientation *= rotate;

        UpdateEulerAngles();
    }

    glm::vec3 TransformQuat::GetFront()
    {
        return Orientation * GlobalFront;
    }

    glm::vec3 TransformQuat::GetUp()
    {
        return Orientation * GlobalUp;
    }

    glm::vec3 TransformQuat::GetRight()
    {
        return Orientation * GlobalRight;
    }

    glm::vec3 TransformQuat::GetPosition()
    {
        return Position;
    }

    glm::quat TransformQuat::GetOrientation()
    {
        return Orientation;
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

    void TransformQuat::SetOrientation(const glm::quat& orientation)
    {
        Orientation = orientation;

        UpdateEulerAngles();
    }

    void TransformQuat::SetPitch(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);

        RotatePitch(angle - Pitch);
    }

    void TransformQuat::SetYaw(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);
        
        RotateYaw(angle - Yaw);
    }

    void TransformQuat::SetRoll(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);
        
        RotateRoll(angle - Roll);
    }

    // privates

    void TransformQuat::UpdateEulerAngles()
    {
        glm::vec3 eulers = glm::eulerAngles(Orientation);
        Pitch = eulers.x;
        Yaw = eulers.y;
        Roll = eulers.z;
    }
}
