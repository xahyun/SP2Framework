#include "gameManager.h"
#include "SceneTemplate.h"
#include "SceneOffice.h"
#include "SceneLoading.h"
#include "SceneFinal.h"
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
	Scene* scene5 = new SceneLoading();
	Scene* scene6 = new SceneFinal();
	scenes.push_back(scene0);
	scenes.push_back(scene1);
	scenes.push_back(scene2);
	scenes.push_back(scene3);
	scenes.push_back(scene4);
	scenes.push_back(scene5);
	scenes.push_back(scene6);
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
			StartGame();
			nextLocation = gameManager::GS_HOME;
			currGameState = gameManager::GS_LOADING;
		}
		if (scene->goNextScene2 == true) {
			isFinishedGame = true;
		}
		break;
	case gameManager::GS_HOME:
		scene = scenes[1];
		if (scene->goNextScene == true) {
			nextLocation = gameManager::GS_WALKINGTOWORK;
			currGameState = gameManager::GS_LOADING;
		}
		if (scene->goNextScene2 == true) {
			FinishGame();
			nextLocation = gameManager::GS_MENU;
			currGameState = gameManager::GS_LOADING;
		}
		break;
	case gameManager::GS_WALKINGTOWORK:
		scene = scenes[2];
		if (scene->goNextScene == true) {
			nextLocation = gameManager::GS_OFFICE;
			currGameState = gameManager::GS_LOADING;
		}
		if (scene->goNextScene2 == true) {
			FinishGame();
			nextLocation = gameManager::GS_MENU;
			currGameState = gameManager::GS_LOADING;
		}
		break;
	case gameManager::GS_OFFICE:
		scene = scenes[0];
		if (scene->goNextScene == true) {
			nextLocation = gameManager::GS_FINAL;
			currGameState = gameManager::GS_LOADING;
		}
		if (scene->goNextScene2 == true) {
			FinishGame();
			nextLocation = gameManager::GS_MENU;
			currGameState = gameManager::GS_LOADING;
		}
		break;
	case gameManager::GS_GOINGBACKHOME:
		break;
	case gameManager::GS_LOADING:
		scene = scenes[5];
		if (scene->goNextScene == true) {
			currGameState = nextLocation;
		}
		break;
	case gameManager::GS_FINAL:
		scene = scenes[6];
		if (scene->goNextScene == true) {
			FinishGame();
			nextLocation = gameManager::GS_MENU;
			currGameState = gameManager::GS_LOADING;
		}
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
	for (Scene* scene : scenes) {
		scene->goNextScene = false;
		scene->goNextScene2 = false;
		scene->Reset();
	}
	//set up leaderboard xx
}

void gameManager::ExitGame()
{
	for (Scene* scene : scenes) {
		scene->Exit();
		delete scene;
	}
}

