#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	//idt we will be using playerpos lol
	Vector3 playerPos;
	float pitch, yaw;
	double xpos, ypos;
	float xOffset, yOffset;
	//if need to stay static
	bool canMove;
	bool canLookAround;

	FirstPersonCamera();
	~FirstPersonCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(float dt);
	virtual void Reset();
	
	//movement functions
	void moveLeft(float dt);
	void moveRight(float dt);
	void moveUp(float dt);
	void moveDown(float dt);

	//camera look functions
	void lookLeft(float dt);
	void lookRight(float dt);
	void lookUp(float dt);
	void lookDown(float dt);

	//special functions
	float angleDifference(const Vector3& point);
};

#endif
