
#include "Camera.h"

#include "Input.h"
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtx/quaternion.hpp>
#include <ext/quaternion_trigonometric.hpp>
#include <ext/quaternion_float.hpp>

namespace GaladHen
{

    Camera::Camera()
        : Transform(TransformQuat{})
        , FovY(45.0f)
        , AspectRatio(1.7f)
        , Near(0.1f)
        , Far(100.0f) // defaults
    {
        Transform.SetYaw(-90.0f); // because of global axes orientations -> in this way the default camera looks towards negative x axis, so potentially in front of a model
        UpdateProjectionMatrix();
    }

    Camera::Camera(const TransformQuat& transform, float fovy, float aspectRatio, float nearDistance, float farDistance)
        : Transform(transform)
        , FovY(fovy)
        , AspectRatio(aspectRatio)
        , Near(nearDistance)
        , Far(farDistance)
    {
        UpdateProjectionMatrix();
    }

    void Camera::ProcessInput(Input& input, float deltaTime)
    {
        // keys
        if (input.GetKeyboardKey(KeyboardKey::W))
            Transform.SetPosition(Transform.GetPosition() + Transform.GetFront() * LinearSpeed * deltaTime);
        if (input.GetKeyboardKey(KeyboardKey::S))
            Transform.SetPosition(Transform.GetPosition() - Transform.GetFront() * LinearSpeed * deltaTime);
        if (input.GetKeyboardKey(KeyboardKey::D))
            Transform.SetPosition(Transform.GetPosition() + Transform.GetRight() * LinearSpeed * deltaTime);
        if (input.GetKeyboardKey(KeyboardKey::A))
            Transform.SetPosition(Transform.GetPosition() - Transform.GetRight() * LinearSpeed * deltaTime);
        if (input.GetKeyboardKey(KeyboardKey::E))
            Transform.SetPosition(Transform.GetPosition() + TransformQuat::GlobalUp * LinearSpeed * deltaTime);
        if (input.GetKeyboardKey(KeyboardKey::Q))
            Transform.SetPosition(Transform.GetPosition() - TransformQuat::GlobalUp * LinearSpeed * deltaTime);

        // rotation
        if (input.GetKeyboardKey(MouseKey::Right))
        {
            // calculate delta radians within limits
            float deltaYaw = -float(AngularSpeed * deltaTime * input.GetDeltaMouseX());
            float deltaPitch = -float(AngularSpeed * deltaTime * input.GetDeltaMouseY());

            // pitch with limits
            float newPitch = Transform.GetPitch() + deltaPitch;
            deltaPitch = glm::clamp(newPitch, -MaxPitchAngle, MaxPitchAngle) - Transform.GetPitch();

            // apply rotation
            Transform.Rotate(deltaPitch, deltaYaw, 0.0f);
        }
    }

    void Camera::SetFovY(float fovy)
    {
        FovY = fovy;

        UpdateProjectionMatrix();
    }

    void Camera::SetAspectRatio(float aspect)
    {
        AspectRatio = aspect;

        UpdateProjectionMatrix();
    }

    void Camera::SetNear(float nearDistance)
    {
        Near = nearDistance;

        UpdateProjectionMatrix();
    }

    void Camera::SetFar(float farDistance)
    {
        Far = farDistance;

        UpdateProjectionMatrix();
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(Transform.GetPosition(), Transform.GetPosition() + Transform.GetFront(), TransformQuat::GlobalUp);;
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {
        return ProjectionMatrix;
    }

    float Camera::GetFovY()
    {
        return FovY;
    }

    float Camera::GetAspectRatio()
    {
        return AspectRatio;
    }

    float Camera::GetNear()
    {
        return Near;
    }

    float Camera::GetFar()
    {
        return Far;
    }

    void Camera::UpdateProjectionMatrix()
    {
        ProjectionMatrix = glm::perspective(glm::radians(FovY), AspectRatio, Near, Far);
    }
}
