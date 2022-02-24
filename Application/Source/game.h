#ifndef GAME_H
#define GAME_H

#include "clock.h"
#include "player.h"
#include "phone.h"

class Game
{
public:
	Clock* gameClock;
	player* gamePlayer;
	Phone* playerPhone;
	float CAM_SPEED = 120;

	Game();
	~Game();
};

#endif