#pragma once
#include "Vertex.h"

struct Light {
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
	Light() {
		position.Set(1, 1, 1);
		color.Set(1, 1, 1);
		power = 1.0f;
		kC = 0.1f, kL = 0.1f, kQ = 0.1f;
	}

	//add these after existing parameters 
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};
