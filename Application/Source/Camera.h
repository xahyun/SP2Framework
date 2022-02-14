#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Application.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	float distance;
	float altitude, azimuth;
	Vector3 view;

	Camera();
	~Camera();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt);
};

#endif