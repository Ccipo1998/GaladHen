/*
Camera class

The camera class could have only few parameters, because many can be calculated starting from the basic ones (ex: view matrix from position, front and up directions;
    position, front and up directions from rotations around world X, Y and Z axis), but here all the data are explicitly saved in memory as an optimization (to avoid to
    perform the same calculations each time, especially for view and projection matrices)
*/

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include "include/glm/gtx/transform.hpp" // for lookat() and perspective()

using namespace std;
using namespace glm;

class Camera
{
    public:

    // movement parameters
    GLfloat LinearSpeed = 1.5f;
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

    Camera(vec3 position, GLfloat roll, GLfloat pitch, GLfloat yaw, GLfloat fovy, GLfloat aspectRatio, GLfloat near, GLfloat far)
    {
        this->Position = position;

        this->Roll = roll;
        this->Pitch = pitch;
        this->Yaw = yaw;

        updateRotationMatrix();

        updateReferenceSystem();

        updateViewMatrix();

        this->FovY = fovy;
        this->AspectRatio = aspectRatio;
        this->Near = near;
        this->Far = far;

        updateProjectionMatrix();
    }

    void applyMovements(bool* keys, GLfloat deltaTime)
    {
        if (keys[GLFW_KEY_UP])
            setPosition(this->Position + this->Front * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_DOWN])
            setPosition(this->Position - this->Front * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_RIGHT])
            setPosition(this->Position + this->Right * this->LinearSpeed * deltaTime);
        if(keys[GLFW_KEY_LEFT])
            setPosition(this->Position - this->Right * this->LinearSpeed * deltaTime);
    }

    // setting functions

    // set a new position for the camera
    void setPosition(vec3 value)
    {
        this->Position = value;

        updateViewMatrix();
    }

    // set a new value for Roll angle
    void setRoll(GLfloat value)
    {
        this->Roll = value;

        updateRotationMatrix();

        updateReferenceSystem();

        updateViewMatrix();
    }

    // set a new value for Pitch angle
    void setPitch(GLfloat value)
    {
        this->Pitch = value;

        updateRotationMatrix();

        updateReferenceSystem();

        updateViewMatrix();
    }

    // set a new value for Yaw angle
    void setYaw(GLfloat value)
    {
        this->Yaw = value;

        updateRotationMatrix();

        updateReferenceSystem();

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

    mat4 getRotationMatrix()
    {
        return this->RotationMatrix;
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
    GLfloat Roll;
    // pitch is the rotation around Y axis
    GLfloat Pitch;
    // yaw is the rotation around Z axis
    GLfloat Yaw;

    vec3 Front;
    vec3 Up;
    vec3 Right;

    mat4 RotationMatrix;

    GLfloat FovY;
    GLfloat AspectRatio;
    GLfloat Near;
    GLfloat Far;

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;

    // calculate new rotation matrix from euler angles
    void updateRotationMatrix()
    {
        vec4 row0 = vec4(cos(this->Yaw) * cos(this->Pitch), cos(this->Yaw) * sin(this->Pitch) * sin(this->Roll) - sin(this->Yaw) * cos(this->Roll), cos(this->Yaw) * sin(this->Pitch) * cos(this->Roll) + sin(this->Yaw) * sin(this->Roll), .0f);
        vec4 row1 = vec4(sin(this->Yaw) * cos(this->Pitch), sin(this->Yaw) * sin(this->Pitch) * sin(this->Roll) + cos(this->Yaw) * cos(this->Roll), sin(this->Yaw) * sin(this->Pitch) * cos(this->Roll) - cos(this->Yaw) * sin(this->Roll), .0f);
        vec4 row2 = vec4(-sin(this->Pitch), cos(this->Pitch) * sin(this->Roll), cos(this->Pitch) * cos(this->Roll), .0f);
        vec4 row3 = vec4(.0f, .0f, .0f, 1.0f);
        this->RotationMatrix = mat4(row0, row1, row2, row3);
    }

    // calculate new reference system from rotation matrix
    void updateReferenceSystem()
    {
        this->Front = this->RotationMatrix[0];
        this->Up = this->RotationMatrix[1];
        this->Right = this->RotationMatrix[2];
    }

    // calculating new euler angles from local reference system
    void updateEulerAngles()
    {
        GLfloat x1_first_x = this->Right.y * this->Front.x - this->Right.x * this->Front.y;
        GLfloat x1_first_y = this->Right.y * this->Up.x - this->Right.x * this->Up.y;
        GLfloat alpha = atan2(x1_first_y, x1_first_x);
        // TODO: finire -> http://geom3d.com/data/documents/Calculation=20of=20Euler=20angles.pdf
        // non sono sicuro che sia l'agoritmo giusto
    }

    // calculate new projection matrix from perspective parameters
    void updateProjectionMatrix()
    {
        this->ProjectionMatrix = perspective(this->FovY, this->AspectRatio, this->Near, this->Far);
    }

    // calculate new view matrix from orientation parameters
    void updateViewMatrix()
    {
        this->ViewMatrix = lookAt(this->Position, this->Position + this->Front, this->Up);
    }
};
