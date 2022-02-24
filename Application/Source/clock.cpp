#include "clock.h"

#define SECONDSPERMINUTE 1.5

Clock::Clock()
{
	time = 0;
	hour = minute = 0;
}

Clock::~Clock()
{
}

//put in update method
void Clock::UpdateClock(double dt)
{
	time += dt;
	if (time > SECONDSPERMINUTE) {
		time = 0;
		minute += 1;
	}
	if (minute >= 60) {
		hour++;
		minute = 0;
	}
	if (hour > 23) {
		hour = 0;
	}
}

std::string Clock::getHourMinute()
{
	std::string time;
	if (hour < 10) {
		time += ("0" + std::to_string(hour));
	}
	else {
		time += std::to_string(hour);
	}
	time += ": ";
	if (minute < 10) {
		time += ("0" + std::to_string(minute));
	}
	else {
		time += std::to_string(minute);
	}
	return time;
}

void Clock::newDay()
{
	hour = 9;
	minute = 0;
	time = 0;
}

void Clock::setTime(int hour, int minute)
{
	this->hour = hour;
	this->minute = minute;
}


