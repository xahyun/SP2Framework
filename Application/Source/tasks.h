#ifndef TASKS_H
#define TASKS_H

#include <string>

struct Tasks
{
	std::string taskName;
	bool isCompleted = false;
	Tasks(std::string name) {
		taskName = name;
		isCompleted = false;
	}
	Tasks() {
		taskName = "";
		isCompleted = false;
	}
};

#endif