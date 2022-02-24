#ifndef CLOCK_H
#define CLOCK_H

#include <string>

class Clock {
private:
	float time;
	int hour;
	int minute;

public:
	Clock();
	~Clock();

	//put in update method
	void UpdateClock(double dt);
	//for printing
	std::string getHourMinute();
	void newDay();
	void setTime(int hour, int minute);
};

#endif