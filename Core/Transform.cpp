
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
        , Scale(glm::vec3(1.0f, 1.0f, 1.0f))
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

    void TransformQuat::RotateGlobal(const glm::quat& rotation)
    {
        Orientation = glm::normalize(rotation * Orientation);
    }

    void TransformQuat::RotateLocal(const glm::quat& rotation)
    {
        Orientation = glm::normalize(Orientation * rotation);
    }

    void TransformQuat::RotatePitch(float deltaPitch)
    {
        glm::quat rotatePitch = glm::angleAxis(glm::radians(deltaPitch), GlobalRight);
        RotateLocal(rotatePitch);

        UpdateEulerAngles();
    }

    void TransformQuat::RotateYaw(float deltaYaw)
    {
        glm::quat rotateYaw = glm::angleAxis(glm::radians(deltaYaw), GlobalUp);

        // i.e. Orientation = glm::normalize(rotation * Orientation):
        // https://stackoverflow.com/questions/9857398/quaternion-camera-how-do-i-make-it-rotate-correctly
        // https://gamedev.stackexchange.com/questions/136174/im-rotating-an-object-on-two-axes-so-why-does-it-keep-twisting-around-the-thir
        // https://gamedev.stackexchange.com/questions/207177/correct-order-on-accumulating-rotations
        // "Pitch Locally, Yaw Globally" mantra
        // YXZ rotation order
        RotateGlobal(rotateYaw);

        UpdateEulerAngles();
    }

    void TransformQuat::RotateRoll(float deltaRoll)
    {
        glm::quat rotateRoll = glm::angleAxis(glm::radians(deltaRoll), GlobalFront);
        RotateLocal(rotateRoll);

        UpdateEulerAngles();
    }

    glm::vec3 TransformQuat::GetFront() const
    {
        return Orientation * GlobalFront;
    }

    glm::vec3 TransformQuat::GetUp() const
    {
        return Orientation * GlobalUp;
    }

    glm::vec3 TransformQuat::GetRight() const
    {
        return Orientation * GlobalRight;
    }

    glm::vec3 TransformQuat::GetPosition() const
    {
        return Position;
    }

    glm::quat TransformQuat::GetOrientation() const
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

    void TransformQuat::SetScale(const glm::vec3& scale)
    {
        Scale = scale;
    }

    void TransformQuat::ScaleX(float scaleX)
    {
        Scale.x = scaleX;
    }

    void TransformQuat::ScaleY(float scaleY)
    {
        Scale.y = scaleY;
    }

    void TransformQuat::ScaleZ(float scaleZ)
    {
        Scale.z = scaleZ;
    }

    glm::vec3 TransformQuat::GetScale() const
    {
        return Scale;
    }

    glm::mat4 TransformQuat::GetModelMatrix() const
    {
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, GetPosition());
        ModelMatrix = glm::scale(ModelMatrix, GetScale());
        return ModelMatrix * glm::toMat4(GetOrientation());
    }

    TransformQuat TransformQuat::Inverse() const
    {
        TransformQuat trans = *this;
        trans.SetOrientation(glm::inverse(GetOrientation()));
        trans.SetPosition(-GetPosition());
        
        return trans;
    }

    // privates
    // TODO: add euler angles management when we have a rotation of 90° -> angles become inaccurate
    void TransformQuat::UpdateEulerAngles()
    {
        glm::vec3 eulers = glm::eulerAngles(Orientation);
        Pitch = glm::degrees(eulers.x);
        Yaw = glm::degrees(eulers.y);
        Roll = glm::degrees(eulers.z);
    }
}
