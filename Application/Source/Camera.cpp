#include "Camera.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
	altitude = 5.f;
	azimuth = 5.f;
	distance = 5.f;
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	altitude = 5.f;
	azimuth = 5.f;
	distance = 5.f;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 5.0f;

	if (Application::IsKeyPressed('W'))
		altitude += CAMERA_SPEED * static_cast<float>(dt); //move camera up
	if (Application::IsKeyPressed('S'))
		altitude -= CAMERA_SPEED * static_cast<float>(dt); //move camera left
	if (Application::IsKeyPressed('A'))
		azimuth -= CAMERA_SPEED * static_cast<float>(dt); //move camera down
	if (Application::IsKeyPressed('D'))
		azimuth += CAMERA_SPEED * static_cast<float>(dt); //move camera right
	if (Application::IsKeyPressed('N'))
		distance -= CAMERA_SPEED * static_cast<float>(dt); //move camera down
	if (Application::IsKeyPressed('M'))
		distance += CAMERA_SPEED * static_cast<float>(dt); //move camera right


	float x = CAMERA_SPEED * distance * cosf(Math::DegreeToRadian(altitude)) * cosf(Math::DegreeToRadian(azimuth));
	float y = CAMERA_SPEED * distance * sinf(Math::DegreeToRadian(altitude));
	float z = CAMERA_SPEED * distance * cosf(Math::DegreeToRadian(altitude)) * sinf(Math::DegreeToRadian(azimuth));

	this->position.x = x; this->position.y = y; this->position.z = z;
}