#include "FirstPersonCamera.h"
#include "Application.h"
#include "Mtx44.h"
#define CAMERA_SPEED 120.f
#define ZOOM_SPEED 5.0f

FirstPersonCamera::FirstPersonCamera()
{
    pitch = 0; yaw = 0;
    xOffset = 0; yOffset = 0;
    xpos = 0; ypos = 0;
    canMove = true;
    canLookAround = true;
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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

void FirstPersonCamera::Update(float dt)
{

    //detect mouse position
    double mousex, mousey;
    Application::getCursorPosition(mousex, mousey);

    //find the offset of the mouse based on last frame
    xOffset = static_cast<float>(mousex - xpos); yOffset = static_cast<float>(mousey - ypos);

    if (canMove) {
        if (Application::IsKeyPressed('A'))
        {
            moveLeft(dt);
        }
        if (Application::IsKeyPressed('D'))
        {
            moveRight(dt);
        }
        if (Application::IsKeyPressed('W'))
        {
            moveUp(dt);
        }
        if (Application::IsKeyPressed('S'))
        {
            moveDown(dt);
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

void FirstPersonCamera::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

void FirstPersonCamera::moveLeft(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = (position - right);
    position.y = defaultPosition.y;
    target = (position + view);
}

void FirstPersonCamera::moveRight(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + right;
    position.y = defaultPosition.y;
    target = position + view;
}

void FirstPersonCamera::moveUp(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position + view;
    position.y = defaultPosition.y;
    target = position + view;
}

void FirstPersonCamera::moveDown(float dt)
{
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    position = position - view;
    position.y = defaultPosition.y;
    target = position + view;
}

float FirstPersonCamera::angleDifference(const Vector3& point)
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

void FirstPersonCamera::lookLeft(float dt) {
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

void FirstPersonCamera::lookRight(float dt) {
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

void FirstPersonCamera::lookDown(float dt) {
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

void FirstPersonCamera::lookUp(float dt) {
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
