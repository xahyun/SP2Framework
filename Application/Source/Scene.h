#ifndef SCENE_H
#define SCENE_H

#include "game.h"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void TransferGameInfo(Game* game) = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	bool goNextScene = false;
	bool goNextScene2 = false;
};

#endif