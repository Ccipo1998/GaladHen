/*
Camera class

The camera class could have only few parameters, because many can be calculated starting from the basic ones (ex: view matrix from position, front and up directions;
    position, front and up directions from rotations around world X, Y and Z axis), but here all the data are explicitly saved in memory as an optimization (to avoid to
    perform the same calculations each time, especially for view and projection matrices)
*/

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtx/quaternion.hpp>
#include <ext/quaternion_trigonometric.hpp>
#include <ext/quaternion_float.hpp>

using namespace std;
using namespace glm;

class Camera
{
    public:

    // movement parameters
    GLfloat LinearSpeed = 2.5f;
    GLfloat AngularSpeed = 1.0f;
    /*
    Camera(vec3 position, vec3 front, vec3 up, GLfloat fovy, GLfloat aspectRatio, GLfloat near, GLfloat far)
    {
        this->Position = position;

        this->Front = normalize(front);
        this->Right = normalize(cross(front, up));
        this->Up = cross(this->Front, this->Right);

        UpdateEulerAngles();

        this->FovY = .0f;
        this->AspectRatio = .0f;
        this->Near = .0f;
        this->Far = .0f;
    }
    */

    Camera(vec3 position, GLfloat fovy, GLfloat aspectRatio, GLfloat nearDistance, GLfloat farDistance)
    {
        this->Position = position;

        this->Pitch = .0f;
        this->Yaw = .0f;
        this->Roll = .0f;
        this->Front = vec3(1.0f, .0f, .0f);
        this->Up = vec3(.0f, 1.0f, .0f);
        this->Right = vec3(.0f, .0f, 1.0f);

        updateViewMatrix();

        this->FovY = fovy;
        this->AspectRatio = aspectRatio;
        this->Near = nearDistance;
        this->Far = farDistance;

        updateProjectionMatrix();
    }

    void applyMovements(bool* keys, bool* mouse_keys, double deltaX, double deltaY, GLfloat deltaTime)
    {
        // keys
        if (keys[GLFW_KEY_UP])
            setPosition(this->Position + this->Front * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_DOWN])
            setPosition(this->Position - this->Front * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_RIGHT])
            setPosition(this->Position + this->Right * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_LEFT])
            setPosition(this->Position - this->Right * this->LinearSpeed * deltaTime);

        // mouse position
        if (mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
        {
            // rotations
            quat rotQuatX = angleAxis(GLfloat(this->AngularSpeed * deltaTime * deltaX), this->Up);
            quat rotQuatY = angleAxis(GLfloat(this->AngularSpeed * deltaTime * deltaY), this->Right);
            quat rotQuat = rotQuatX * rotQuatY;
            this->Front = rotQuat * this->Front;
            this->Up = rotQuat * this->Up;
            this->Right = cross(this->Front, this->Up);

            // update yaw, pitch and roll
            vec3 eulers = eulerAngles(rotQuat);
            this->Pitch = degrees(eulers.x);
            this->Yaw = degrees(eulers.y) + 90.0f; // adding 90 degrees (1,5708 radians) because of the opengl standard orientation
            this->Roll = degrees(eulers.z);

            updateViewMatrix();
        }
    }

    // change the orientation to look at the specified position
    void lookAt(vec3 position_front, vec3 up_direction)
    {
        this->Front = normalize(position_front - this->Position);
        this->Right = cross(this->Front, up_direction);
        this->Up = cross(this->Right, this->Front);
        
        updateViewMatrix();

        quat rotQuat = conjugate(toQuat(this->ViewMatrix));
        vec3 eulers = eulerAngles(rotQuat);
        this->Pitch = degrees(eulers.x);
        this->Yaw = degrees(eulers.y) + 90.0f; // adding 90 degrees (1,5708 radians) because of the opengl standard orientation
        this->Roll = degrees(eulers.z);
        
    }

    // setting functions

    // set a new position for the camera
    void setPosition(vec3 value)
    {
        this->Position = value;

        updateViewMatrix();
    }

    // set a new value for Pitch angle
    void setPitch(GLfloat value)
    {
        value = fmod(value, 360.0f);
        quat rotQuat = angleAxis(radians(value - this->Pitch), vec3(1.0f, .0f, .0f));
        this->Front = rotQuat * this->Front;
        this->Up = rotQuat * this->Up;
        this->Right = cross(this->Front, this->Up);
        this->Pitch = value;

        updateViewMatrix();
    }

    // set a new value for Yaw angle
    void setYaw(GLfloat value)
    {
        value = fmod(value, 360.0f);
        quat rotQuat = angleAxis(radians(value - this->Yaw), vec3(.0f, 1.0f, .0f));
        this->Front = rotQuat * this->Front;
        this->Up = rotQuat * this->Up;
        this->Right = cross(this->Front, this->Up);
        this->Yaw = value;

        updateViewMatrix();
    }

    // set a new value for Roll angle
    void setRoll(GLfloat value)
    {
        value = fmod(value, 360.0f);
        quat rotQuat = angleAxis(radians(value - this->Roll), vec3(.0f, .0f, 1.0f));
        this->Front = rotQuat * this->Front;
        this->Up = rotQuat * this->Up;
        this->Right = cross(this->Front, this->Up);
        this->Roll = value;

        updateViewMatrix();
    }

    // set a new value for the fov on Y axis
    void setFovY(GLfloat value)
    {
        this->FovY = value;

        updateProjectionMatrix();
    }

    // set a new value for the aspect ratio
    void setAspectRatio(GLfloat value)
    {
        this->AspectRatio = value;

        updateProjectionMatrix();
    }

    // set a new value for the near plane distance
    void setNear(GLfloat value)
    {
        this->Near = value;

        updateProjectionMatrix();
    }

    // set a new value for the far plane distance
    void setFar(GLfloat value)
    {
        this->Far = value;

        updateProjectionMatrix();
    }

    // getting functions

    vec3 getPosition()
    {
        return this->Position;
    }

    GLfloat getRoll()
    {
        return this->Roll;
    }

    GLfloat getPitch()
    {
        return this->Pitch;
    }

    GLfloat getYaw()
    {
        return this->Yaw;
    }

    vec3 getFront()
    {
        return this->Front;
    }

    vec3 getUp()
    {
        return this->Up;
    }

    vec3 getRight()
    {
        return this->Right;
    }

    mat4 getViewMatrix()
    {
        return this->ViewMatrix;
    }

    GLfloat getFovY()
    {
        return this->FovY;
    }

    GLfloat getAspectRatio()
    {
        return this->AspectRatio;
    }

    GLfloat getNear()
    {
        return this->Near;
    }

    GLfloat getFar()
    {
        return this->Far;
    }

    mat4 getProjectionMatrix()
    {
        return this->ProjectionMatrix;
    }

    private:

    // these data are private because there are specific function to handle the changes (because the changes could interest some other data)

    vec3 Position;

    // roll is the rotation around X axis
    GLfloat Pitch;
    // pitch is the rotation around Y axis
    GLfloat Yaw;
    // yaw is the rotation around Z axis
    GLfloat Roll;

    vec3 Front;
    vec3 Up;
    vec3 Right;

    GLfloat FovY;
    GLfloat AspectRatio;
    GLfloat Near;
    GLfloat Far;

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;

    // calculate new projection matrix from perspective parameters
    void updateProjectionMatrix()
    {
        this->ProjectionMatrix = perspective(this->FovY, this->AspectRatio, this->Near, this->Far);
    }

    // calculate new view matrix from orientation parameters
    void updateViewMatrix()
    {
        this->ViewMatrix = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }
};
