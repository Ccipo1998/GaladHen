
#include "Camera.h"

#include <glm/gtx/transform.hpp> // for lookat() and perspective()
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{

    Camera::Camera()
        : Transform(TransformQuat{})
        , FovY(45.0f)
        , AspectRatio(1.7f)
        , Near(0.1f)
        , Far(100.0f) // defaults
    {
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

    void Camera::ApplyCameraMovements(glm::vec3 linearMovement, glm::vec2 angularMovement, float deltaTime)
    {
        // Linear movement
        if (glm::length(linearMovement) > 0.0f)
        {
            glm::vec3 linearNormalized = glm::normalize(linearMovement);
            glm::vec3 movement = (Transform.GetFront() * linearNormalized.z + Transform.GetRight() * linearNormalized.x + Transform.GetUp() * linearNormalized.y)
                * LinearSpeed * deltaTime;
            Transform.SetPosition(Transform.GetPosition() + movement);
        }

        // Angular movement 

        // calculate delta radians within limits
        float deltaYaw = -float(AngularSpeed * deltaTime * angularMovement.x);
        float deltaPitch = -float(AngularSpeed * deltaTime * angularMovement.y);

        // pitch with limits
        float newPitch = Transform.GetPitch() + deltaPitch;
        deltaPitch = glm::clamp(newPitch, -MaxPitchAngle, MaxPitchAngle) - Transform.GetPitch();

        // apply rotation
        Transform.Rotate(deltaPitch, deltaYaw, 0.0f);
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

    glm::mat4 Camera::GetViewMatrix() const
    {
        glm::vec3 test = Transform.GetFront();
        return glm::lookAt(Transform.GetPosition(), Transform.GetPosition() + Transform.GetFront(), TransformQuat::GlobalUp);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return ProjectionMatrix;
    }

    glm::mat4 Camera::GetNormalMatrix() const
    {
        return glm::transpose(glm::inverse(GetViewMatrix()));
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
