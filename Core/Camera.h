
// The camera class could have only few parameters, because many can be calculated starting from the basic ones (ex: view matrix from position, front and up directions;
//     position, front and up directions from rotations around world X, Y and Z axis), but here all the data are explicitly saved in memory as an optimization (to avoid to
//     perform the same calculations each time, especially for view and projection matrices)

#pragma once

#include <glm/glm.hpp>

#include "transform.h"

namespace GaladHen
{
    class Camera
    {
    public:

        TransformQuat Transform;

        // movement parameters
        float LinearSpeed = 5.0f;
        float AngularSpeed = 500.0f;
        float MaxPitchAngle = 80.0f;

        Camera();
        
        Camera(const TransformQuat& transform, float fovy, float aspectRatio, float nearDistance, float farDistance);

        // Copy constructor, Copy assignment, Move constructor and Move assignment are defaulted

        // @brief
        // Move and rotate camera, within limits
        // @param linearMovement: vector representing movement of camera position on the three axes
        // @param angularMovement: vector representing movement of camera orietation (Yaw, Pitch)
        void ApplyCameraMovements(glm::vec3 linearMovement, glm::vec2 angularMovement, float deltaTime);

        // setters

        void SetFovY(float fovy);
        void SetAspectRatio(float aspect);
        void SetNear(float nearDistance);
        void SetFar(float farDistance);

        // getters

        glm::mat4 GetViewMatrix() const; // the transform is used to calculate the view matrix
        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetNormalMatrix() const;

        float GetFovY();
        float GetAspectRatio();
        float GetNear();
        float GetFar();

    private:

        // these data are private because there are specific function to handle the changes (because the changes could interest some other data)

        float FovY;
        float AspectRatio;
        float Near;
        float Far;

        glm::mat4 ProjectionMatrix;

        // @brief
        // Calculate new projection matrix basing on perspective parameters
        void UpdateProjectionMatrix();
    };
}
