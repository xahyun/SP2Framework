#include "CameraCity.h"
#include "Application.h"
#include "Mtx44.h"
#define ZOOM_SPEED 5.0f

CameraCity::CameraCity()
{
    pitch = 0; yaw = 0;
    xOffset = 0; yOffset = 0;
    xpos = 0; ypos = 0;
    canMove = true;
    canLookAround = true;
}

CameraCity::~CameraCity()
{
}

void CameraCity::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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

void CameraCity::Update(float dt)
{

    //detect mouse position
    double mousex, mousey;
    Application::getCursorPosition(mousex, mousey);

    //find the offset of the mouse based on last frame
    xOffset = static_cast<float>(mousex - xpos); yOffset = static_cast<float>(mousey - ypos);

    //if (canMove) {
    //    if (Application::IsKeyPressed('A'))
    //    {
    //        moveLeft(dt);
    //    }
    //    if (Application::IsKeyPressed('D'))
    //    {
    //        moveRight(dt);
    //    }
    //    if (Application::IsKeyPressed('W'))
    //    {
    //        moveUp(dt);
    //    }
    //    if (Application::IsKeyPressed('S'))
    //    {
    //        moveDown(dt);
    //    }
    //} 
    
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);


    if (canMove)
    {
        if (Application::IsKeyPressed('A'))
        {
            moveLeft(dt);
            position = position - right * ZOOM_SPEED * dt;
            if (position.x < -245)
            {
                position.x = -245;
            }
            if (position.x > 245)
            {
                position.x = 245;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 395)
            {
                position.z = 395;
            }
            float dmin, dmax;
            if ((position.x > -155 && position.x < 150) && (position.z > 23 && position.z < 284))
            {
                dmin = -155 - position.x;
                dmax = 150 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -156;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 151;
                }

                dmin = 23 - position.z;
                dmax = 284 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 22;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
            //red car 
            if ((position.x > 208 && position.x < 235) && (position.z > 47 && position.z < 105))
            {
                dmin = 208 - position.x;
                dmax = 235 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 207;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 236;
                }

                dmin = 47 - position.z;
                dmax = 105 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 106;
                }
            }
            //blue car
            if ((position.x > 8 && position.x < 55) && (position.z > -41 && position.z < -19))
            {
                dmin = 8 - position.x;
                dmax = 55 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 7;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 56;
                }

                dmin = -41 - position.z;
                dmax = -19 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -42;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = -18;
                }
            }
            //police

            if ((position.x > -200 && position.x < -178) && (position.z > 47 && position.z < 96))
            {
                dmin = -200 - position.x;
                dmax = -178 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -201;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -177;
                }

                dmin = 47 - position.z;
                dmax = 96 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 97;
                }
            }
            //taxi
            if ((position.x > -76 && position.x < -28) && (position.z > 343 && position.z < 365))
            {
                dmin = -76 - position.x;
                dmax = -28 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -77;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -27;
                }

                dmin = 343 - position.z;
                dmax = 365 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 342;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 366;
                }
            }



            target = position + view;
        }
        if (Application::IsKeyPressed('D'))
        {
            moveRight(dt);
            position = position + right * ZOOM_SPEED * dt;
            if (position.x < -245)
            {
                position.x = -245;
            }
            if (position.x > 245)
            {
                position.x = 245;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 395)
            {
                position.z = 395;
            }

            float dmin, dmax;
            if ((position.x > -155 && position.x < 150) && (position.z > 23 && position.z < 284))
            {
                dmin = -155 - position.x;
                dmax = 150 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -156;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 151;
                }

                dmin = 23 - position.z;
                dmax = 284 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 22;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
            //red car 
            if ((position.x > 208 && position.x < 235) && (position.z > 47 && position.z < 105))
            {
                dmin = 208 - position.x;
                dmax = 235 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 207;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 236;
                }

                dmin = 47 - position.z;
                dmax = 105 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 106;
                }
            }
            //blue car
            if ((position.x > 8 && position.x < 55) && (position.z > -41 && position.z < -19))
            {
                dmin = 8 - position.x;
                dmax = 55 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 7;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 56;
                }

                dmin = -41 - position.z;
                dmax = -19 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -42;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = -18;
                }
            }
            //police

            if ((position.x > -200 && position.x < -178) && (position.z > 47 && position.z < 96))
            {
                dmin = -200 - position.x;
                dmax = -178 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -201;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -177;
                }

                dmin = 47 - position.z;
                dmax = 96 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 97;
                }
            }
            //taxi
            if ((position.x > -76 && position.x < -28) && (position.z > 343 && position.z < 365))
            {
                dmin = -76 - position.x;
                dmax = -28 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -77;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -27;
                }

                dmin = 343 - position.z;
                dmax = 365 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 342;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 366;
                }
            }
        }

            target = position + view;
    
        if (Application::IsKeyPressed('W'))
        {
            moveUp(dt);
            position = position + view * ZOOM_SPEED * dt;
            if (position.x < -245)
            {
                position.x = -245;
            }
            if (position.x > 245)
            {
                position.x = 245;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 395)
            {
                position.z = 395;
            }
            if ((position.x < 150 && 
                position.x > -155) &&
                position.z > 23 &&
                position.z < 284 )
            {
                std::cout << "W" << std::endl;
            }
         
            float dmin, dmax;
            if ((position.x > -155 && position.x < 150) && (position.z > 23 && position.z < 284))
            {
                dmin = -155 - position.x;
                dmax = 150 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -156;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 151;
                }

                dmin = 23 - position.z;
                dmax = 284 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 22;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
            //red car 
            if ((position.x > 208 && position.x < 235) && (position.z > 47 && position.z < 105))
            {
                dmin = 208 - position.x;
                dmax = 235 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 207;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 236;
                }

                dmin = 47 - position.z;
                dmax = 105 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 106;
                }
            }
            //blue car
            if ((position.x > 8 && position.x < 55) && (position.z > -41 && position.z < -19))
            {
                dmin = 8 - position.x;
                dmax = 55 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 7;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 56;
                }

                dmin = -41 - position.z;
                dmax = -19 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -42;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = -18;
                }
            }
            //police

            if ((position.x > -200 && position.x < -178) && (position.z > 47 && position.z < 96))
            {
                dmin = -200 - position.x;
                dmax = -178 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -201;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -177;
                }

                dmin = 47 - position.z;
                dmax = 96 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 97;
                }
            }
            //taxi
            if ((position.x > -76 && position.x < -28) && (position.z > 343 && position.z < 365))
            {
                dmin = -76 - position.x;
                dmax = -28 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -77;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -27;
                }

                dmin = 343 - position.z;
                dmax = 365 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 342;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 366;
                }
            }
            target = position + view;
        }
        if (Application::IsKeyPressed('S'))
        {
            moveDown(dt);
            position = position - view * ZOOM_SPEED * dt;
            if (position.x < -245)
            {
                position.x = -245;
            }
            if (position.x > 245)
            {
                position.x = 245;
            }
            if (position.z < -60)
            {
                position.z = -60;
            }
            if (position.z > 395)
            {
                position.z = 395;
            }
            float dmin, dmax;
            if ((position.x > -155 && position.x < 150) && (position.z > 23 && position.z < 284))
            {
                dmin = -155 - position.x;
                dmax = 150 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -156;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 151;
                }

                dmin = 23 - position.z;
                dmax = 284 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 22;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 285;
                }
            }
           //red car 
            if ((position.x > 208 && position.x < 235) && (position.z > 47 && position.z < 105))
            {
                dmin = 208 - position.x;
                dmax = 235 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 207;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 236;
                }

                dmin = 47 - position.z;
                dmax = 105 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 106;
                }
            }
            //blue car
            if ((position.x > 8 && position.x < 55) && (position.z > -41 && position.z < -19))
            {
                dmin = 8 - position.x;
                dmax = 55 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = 7;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = 56;
                }

                dmin = -41 - position.z;
                dmax = -19 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = -42;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = -18;
                }
            }
            //police

            if ((position.x > -200 && position.x < -178) && (position.z > 47 && position.z < 96))
            {
                dmin = -200 - position.x;
                dmax = -178 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -201;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -177;
                }

                dmin = 47 - position.z;
                dmax = 96 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 46;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 97;
                }
            }
            //taxi
            if ((position.x > -76 && position.x < -28) && (position.z > 343 && position.z < 365))
            {
                dmin = -76 - position.x;
                dmax = -28 - position.x;

                if (dmin<0 && dmin>-1)
                {
                    position.x = -77;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.x = -27;
                }

                dmin = 343 - position.z;
                dmax = 365 - position.z;
                if (dmin<0 && dmin>-1)
                {
                    std::cout << dmin << std::endl;
                    position.z = 342;
                }
                else if (dmax > 0 && dmax < 1)
                {
                    position.z = 366;
                }
            }
            target = position + view;
        }
    }


    if (canLookAround) {
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

    xpos = mousex; ypos = mousey;

   
}

void CameraCity::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void CameraCity::moveLeft(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = (position - right);
    position.y = defaultPosition.y;
    target = (position + view);
}

void CameraCity::moveRight(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + right;
    position.y = defaultPosition.y;
    target = position + view;
}

void CameraCity::moveUp(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + view;
    position.y = defaultPosition.y;
    target = position + view;
}

void CameraCity::moveDown(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position - view;
    position.y = defaultPosition.y;
    target = position + view;
}

float CameraCity::angleDifference(const Vector3& point)
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

void CameraCity::lookLeft(float dt) {
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

void CameraCity::lookRight(float dt) {
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

void CameraCity::lookDown(float dt) {
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

void CameraCity::lookUp(float dt) {
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