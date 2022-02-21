#ifndef PLAYER_H
#define PLAYER_H
#define DEFAULTMONEY 10000
#define DEFAULTSOCIALMETER 50.0

#include <string>
#include <stack>
#include "tasks.h"

class player {
private:
	int money;
	float socialMeter;

public:
	player();
	player(int money, float socialMeter);
	~player();

	//getters and setters
	int getMoney();
	float getSocialMeter();

	void setMoney(long money);
	void setSocialMeter(float socialMeter);

	void addMoney(int difference);
	void addSocialMeter(float difference);

	//others
	float getScore();
};

#endif