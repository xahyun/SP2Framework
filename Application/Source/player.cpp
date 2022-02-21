#include "player.h"

player::player()
{
	money = DEFAULTMONEY;
	socialMeter = DEFAULTSOCIALMETER;
}

player::player(int money, float socialMeter)
{
	setMoney(money);
	setSocialMeter(socialMeter);
}

player::~player()
{
}

int player::getMoney()
{
	return money;
}

float player::getSocialMeter()
{
	return socialMeter;
}

void player::setMoney(long money)
{
	if (money >= INT16_MAX) {
		this->money = INT16_MAX;
	}
	else if (money <= INT16_MIN) {
		this->money = INT16_MIN;
	}
	else {
		this->money = money;
	}
}

void player::setSocialMeter(float socialMeter)
{
	if (socialMeter > 100.0f) {
		this->socialMeter = 100.0f;
	}
	else if (socialMeter < 1.0f) {
		this->socialMeter = 1.0f;
	}
	else {
		this->socialMeter = socialMeter;
	}
}

void player::addMoney(int difference)
{
	setMoney(money + difference);
}

void player::addSocialMeter(float difference)
{
	setSocialMeter(socialMeter + difference);
}

float player::getScore()
{
	if (money >= 0) {
		return ((money * socialMeter) / 100);
	}
	else {
		return (money / socialMeter);
	}
}
