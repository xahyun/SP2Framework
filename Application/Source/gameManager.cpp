#include "gameManager.h"
#include "SceneTemplate.h"
#include "SceneOffice.h"
#include "SceneCity.h"
#include "SceneHome.h"
#include "SceneMenu.h"

gameManager::gameManager()
{
	isFinishedGame = false;
	currGameState = GS_MENU;
	dayCount = 0;
	
}

gameManager::~gameManager()
{
}

void gameManager::Init()
{
	Scene* scene0 = new SceneOffice();
	Scene* scene1 = new SceneHome();
	Scene* scene2 = new SceneCity();
	Scene* scene3 = new SceneTemplate();
	Scene* scene4 = new SceneMenu();
	scenes.push_back(scene0);
	scenes.push_back(scene1);
	scenes.push_back(scene2);
	scenes.push_back(scene3);
	scenes.push_back(scene4);
	for (Scene* scene : scenes) {
		scene->Init();
	}
}

void gameManager::Update(double dt)
{

	switch (currGameState)
	{
	case gameManager::GS_MENU:
		scene = scenes[4];
		if (scene->goNextScene == true) {
			currGameState = gameManager::GS_HOME;
		}
		if (scene->goNextScene2 == true) {
			isFinishedGame = true;
		}
		break;
	case gameManager::GS_HOME:
		scene = scenes[1];
		if (scene->goNextScene == true) {
			currGameState = gameManager::GS_WALKINGTOWORK;
		}
		break;
	case gameManager::GS_WALKINGTOWORK:
		scene = scenes[2];
		if (scene->goNextScene == true) {
			currGameState = gameManager::GS_OFFICE;
		}
		break;
	case gameManager::GS_OFFICE:
		scene = scenes[0];
		break;
	case gameManager::GS_GOINGBACKHOME:
		break;
	}
	
	if (game != NULL) {
		scene->TransferGameInfo(game);
	}

	scene->Update(dt);
	scene->Render();
}

void gameManager::StartGame()
{
	game = new Game();
	dayCount = 0;
}

void gameManager::FinishGame()
{
	delete game;
	//set up leaderboard xx
}

void gameManager::ExitGame()
{
	for (Scene* scene : scenes) {
		scene->Exit();
		delete scene;
	}
}

