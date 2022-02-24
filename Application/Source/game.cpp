#include "game.h"

Game::Game()
{
	gameClock = new Clock();
	gameClock->newDay();
	gamePlayer = new player();
	playerPhone = new Phone();
}

Game::~Game()
{
	delete gamePlayer;
	delete playerPhone;
}
