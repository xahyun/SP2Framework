#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <string>
#include <utility>
#include <vector>
#include <stack>

#include "Scene.h"
//include other scenes

#include "game.h"

//this class is more of a scene manager tbh
class gameManager {
public:
	enum gameState {
		GS_MENU,
		GS_HOME,
		GS_WALKINGTOWORK,
		GS_OFFICE,
		GS_EMAIL,
		GS_GOINGBACKHOME,
		GS_TOTALNUM
	};

	//public variables
	gameState currGameState;
	int dayCount;
	std::vector<Scene*> scenes; Scene* scene;
	bool isFinishedGame;
	Game* game;

	gameManager();
	~gameManager();

	//load game up -> load all scenes ??
	void Init();
	//run game
	void Update(double dt);
	//start level
	void StartGame();
	//when cycle over
	void FinishGame();
	void ExitGame();

};

#endif