
#include "Transform.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{
    const glm::vec3 Transform::GlobalFront = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 Transform::GlobalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 Transform::GlobalRight = glm::vec3(0.0f, 0.0f, 1.0f);

    Transform::Transform()
        : Position(glm::vec3{})
        , Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Scale(glm::vec3(1.0f, 1.0f, 1.0f))
        , Pitch(0.0f)
        , Yaw(0.0f)
        , Roll(0.0f)
    {}

    void Transform::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
    {
        RotatePitch(deltaPitch);
        RotateYaw(deltaYaw);
        RotateRoll(deltaRoll);
    }

    void Transform::RotateGlobal(const glm::quat& rotation)
    {
        Orientation = glm::normalize(rotation * Orientation);
    }

    void Transform::RotateLocal(const glm::quat& rotation)
    {
        Orientation = glm::normalize(Orientation * rotation);
    }

    void Transform::RotatePitch(float deltaPitch)
    {
        glm::quat rotatePitch = glm::angleAxis(glm::radians(deltaPitch), GlobalRight);
        RotateLocal(rotatePitch);

        UpdateEulerAngles();
    }

    void Transform::RotateYaw(float deltaYaw)
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

    void Transform::RotateRoll(float deltaRoll)
    {
        glm::quat rotateRoll = glm::angleAxis(glm::radians(deltaRoll), GlobalFront);
        RotateLocal(rotateRoll);

        UpdateEulerAngles();
    }

    glm::vec3 Transform::GetFront() const
    {
        return Orientation * GlobalFront;
    }

    glm::vec3 Transform::GetUp() const
    {
        return Orientation * GlobalUp;
    }

    glm::vec3 Transform::GetRight() const
    {
        return Orientation * GlobalRight;
    }

    glm::vec3 Transform::GetPosition() const
    {
        return Position;
    }

    glm::quat Transform::GetOrientation() const
    {
        return Orientation;
    }

    float Transform::GetPitch()
    {
        return Pitch;
    }

    float Transform::GetYaw()
    {
        return Yaw;
    }

    float Transform::GetRoll()
    {
        return Roll;
    }

    void Transform::SetPosition(const glm::vec3& position)
    {
        Position = position;
    }

    void Transform::SetOrientation(const glm::quat& orientation)
    {
        Orientation = orientation;

        UpdateEulerAngles();
    }

    void Transform::SetPitch(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);

        RotatePitch(angle - Pitch);
    }

    void Transform::SetYaw(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);

        RotateYaw(angle - Yaw);
    }

    void Transform::SetRoll(float angle)
    {
        angle = glm::mod(angle, 359.9999999f);

        RotateRoll(angle - Roll);
    }

    void Transform::SetScale(const glm::vec3& scale)
    {
        Scale = scale;
    }

    void Transform::ScaleX(float scaleX)
    {
        Scale.x = scaleX;
    }

    void Transform::ScaleY(float scaleY)
    {
        Scale.y = scaleY;
    }

    void Transform::ScaleZ(float scaleZ)
    {
        Scale.z = scaleZ;
    }

    glm::vec3 Transform::GetScale() const
    {
        return Scale;
    }

    glm::mat4 Transform::ToMatrix() const
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, GetPosition());
        matrix = glm::scale(matrix, GetScale());
        return matrix * glm::toMat4(GetOrientation());
    }

    Transform Transform::Inverse() const
    {
        Transform trans = *this;
        trans.SetOrientation(glm::inverse(GetOrientation()));
        trans.SetPosition(-GetPosition());

        return trans;
    }

    void Transform::LookAt(const glm::vec3& position)
    {
        glm::vec3 direction = glm::normalize(position - Position);

        glm::quat rot = glm::angleAxis(glm::radians(90.0f), GlobalUp); // because of the difference between glm and GaladHen reference system orientation
        if (glm::dot(direction, GlobalUp) > 0.9999f)
        {
            rot = glm::quatLookAt(direction, -GlobalFront) * rot; // order matters!
        }
        else
        {
            rot = glm::quatLookAt(direction, GlobalUp) * rot; // order matters!
        }

        SetOrientation(rot);
    }

    // privates
    // TODO: add euler angles management when we have a rotation of 90° -> angles become inaccurate
    void Transform::UpdateEulerAngles()
    {
        glm::vec3 eulers = glm::eulerAngles(Orientation);
        Pitch = glm::degrees(eulers.x);
        Yaw = glm::degrees(eulers.y);
        Roll = glm::degrees(eulers.z);
    }
}
