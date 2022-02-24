#include "CameraHome.h"
#include "Application.h"
#include "Mtx44.h"
#define ZOOM_SPEED 5.0f

CameraHome::CameraHome()
{
    pitch = 0; yaw = 0;
    xOffset = 0; yOffset = 0;
    xpos = 0; ypos = 0;
    canMove = true;
    canLookAround = true;
}

CameraHome::~CameraHome()
{
}

void CameraHome::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    Application::setCursorPos(400, 300);
    Application::getCursorPosition(xpos, ypos);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
}

void CameraHome::Update(float dt)
{

    //detect mouse position
    double mousex, mousey;
    Application::getCursorPosition(mousex, mousey);

    //find the offset of the mouse based on last frame
    xOffset = static_cast<float>(mousex - xpos); yOffset = static_cast<float>(mousey - ypos);

    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);

    if (canMove)
    {

        if (Application::IsKeyPressed('A'))
        {
            moveLeft(dt);
            position = position - right * ZOOM_SPEED * dt;
            if (position.x < -61)
            {
                position.x = -61;
            }
            if (position.x > 61)
            {
                position.x = 61;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 95)
            {
                position.z = 95;
            }
            float dmin, dmax;
            if ((position.x > -60 && position.x < -8) && (position.z > 42 && position.z < 95))
            {
                dmin = -60 - position.x;
                dmax = -8 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -61;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -7;
                }

                dmin = 42 - position.z;
                dmax = 95 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 41;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
            //sofa

            if ((position.x > -55 && position.x < -14) && (position.z > -34 && position.z < -1))
            {
                dmin = -55 - position.x;
                dmax = -14 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -56;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -13;
                }

                dmin = -34 - position.z;
                dmax = -1 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -35;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 0;
                }
            }

            target = position + view;
        }
        if (Application::IsKeyPressed('D'))
        {
            moveRight(dt);
            position = position + right * ZOOM_SPEED * dt;
            if (position.x < -61)
            {
                position.x = -61;
            }
            if (position.x > 61)
            {
                position.x = 61;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 95)
            {
                position.z = 95;
            }
            float dmin, dmax;
            if ((position.x > -60 && position.x < -8) && (position.z > 42 && position.z < 95))
            {
                dmin = -60 - position.x;
                dmax = -8 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -61;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -7;
                }

                dmin = 42 - position.z;
                dmax = 95 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 41;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }

            }
            //sofa

            if ((position.x > -55 && position.x < -14) && (position.z > -34 && position.z < -1))
            {
                dmin = -55 - position.x;
                dmax = -14 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -56;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -13;
                }

                dmin = -34 - position.z;
                dmax = -1 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -35;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 0;
                }
            }



            target = position + view;
        }
        if (Application::IsKeyPressed('W'))
        {
            moveUp(dt);
            position = position + view * ZOOM_SPEED * dt;
            if (position.x < -61)
            {
                position.x = -61;
            }
            if (position.x > 61)
            {
                position.x = 61;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 95)
            {
                position.z = 95;
            }
            float dmin, dmax;
            if ((position.x > -60 && position.x < -8) && (position.z > 42 && position.z < 95))
            {
                dmin = -60 - position.x;
                dmax = -8 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -61;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -7;
                }

                dmin = 42 - position.z;
                dmax = 95 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 41;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
            //sofa

            if ((position.x > -55 && position.x < -14) && (position.z > -34 && position.z < -1))
            {
                dmin = -55 - position.x;
                dmax = -14 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -56;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -13;
                }

                dmin = -34 - position.z;
                dmax = -1 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -35;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 0;
                }
            }

            target = position + view;
        }
        if (Application::IsKeyPressed('S'))
        {
            moveDown(dt);
            position = position - view * ZOOM_SPEED * dt;
            if (position.x < -61)
            {
                position.x = -61;
            }
            if (position.x > 61)
            {
                position.x = 61;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 95)
            {
                position.z = 95;
            }
            float dmin, dmax;
            if ((position.x > -60 && position.x < -8) && (position.z > 42 && position.z < 95))
            {
                dmin = -60 - position.x;
                dmax = -8 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -61;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -7;
                }

                dmin = 42 - position.z;
                dmax = 95 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 41;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }

            //sofa

            if ((position.x > -55 && position.x < -14) && (position.z > -34 && position.z < -1))
            {
                dmin = -55 - position.x;
                dmax = -14 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -56;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -13;
                }

                dmin = -34 - position.z;
                dmax = -1 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -35;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 0;
                }
            }



            target = position + view;
        }


        if (canLookAround)
        {
            if (xOffset < -1)
            {
                lookLeft(dt);
            }
            else if (xOffset > 1)
            {
                lookRight(dt);
            }
            if (yOffset < -1)
            {
                lookDown(dt);
            }
            else if (yOffset > 1)
            {
                lookUp(dt);
            }
        }


    }   xpos = mousex; ypos = mousey;
}

void CameraHome::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void CameraHome::moveLeft(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = (position - right);
    position.y = defaultPosition.y;
    target = (position + view);
}

void CameraHome::moveRight(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + right;
    position.y = defaultPosition.y;
    target = position + view;
}

void CameraHome::moveUp(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + view;
    position.y = defaultPosition.y;
    target = position + view;
}

void CameraHome::moveDown(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position - view;
    position.y = defaultPosition.y;
    target = position + view;
}

float CameraHome::angleDifference(const Vector3& point)
{
    Vector3 v1 = (target - position).Normalized();
    Vector3 v2 = point;
    v2.y = v1.y;
    float basicAngle = v1.AngleDifference(v2);
    //check if is on left or right
    if (v1.x - v2.x > 0) {
        return basicAngle;
    }
    else {
        return -basicAngle;
    }

}

void CameraHome::lookLeft(float dt) {
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();

    float yaw = CAMERA_SPEED * static_cast<float>(dt);
    //float yaw = xpos;
    up = right.Cross(view).Normalized();

    Mtx44 rotation;
    rotation.SetToRotation(yaw, up.x, up.y, up.z);

    view = rotation * view;
    target = position + view;
}

void CameraHome::lookRight(float dt) {
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);

    right.y = 0;
    right.Normalize();

    float yaw = -CAMERA_SPEED * static_cast<float>(dt);
    up = right.Cross(view).Normalized();

    Mtx44 rotation;
    rotation.SetToRotation(yaw, up.x, up.y, up.z);

    view = rotation * view;
    target = position + view;
}

void CameraHome::lookDown(float dt) {
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();

    float pitch = CAMERA_SPEED * static_cast<float>(dt);
    //float pitch = 0;
    up = right.Cross(view).Normalized();

    Mtx44 rotation;
    rotation.SetToRotation(pitch, right.x, right.y, right.z);

    if (view.y < -sin(70)) {
        view.y = static_cast<float>(-sin(70));
    }
    if (view.y > sin(70)) {
        view.y = static_cast<float>(sin(70));
    }

    view = rotation * view;
    target = position + view;
}

void CameraHome::lookUp(float dt) {
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();

    float pitch = -CAMERA_SPEED * static_cast<float>(dt);
    up = right.Cross(view).Normalized();

    Mtx44 rotation;
    rotation.SetToRotation(pitch, right.x, right.y, right.z);

    if (view.y < -sin(70)) {
        view.y = static_cast<float>(-sin(70));
    }
    if (view.y > sin(70)) {
        view.y = static_cast<float>(sin(70));
    }

    view = rotation * view;
    target = position + view;
}