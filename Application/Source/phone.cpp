#include "phone.h"

Phone::Phone()
{
	timer = 0;
	randomNumber = 0;
	addTaskToList("");
}

Phone::~Phone()
{
}

void Phone::Update(float dt)
{
	if (timer > randomNumber) {
		timer = 0;
		//generate random chats
		randomNumber = Math::RandIntMinMax(30, 60);
		GenerateRandomChats();
	}
}

void Phone::GenerateRandomChats()
{
	int chatNumber = Math::RandIntMinMax(0, existingChats.size());
	chats.push_back(existingChats[chatNumber]);

	//remove from existing chats
	existingChats.erase(existingChats.begin() + chatNumber);
}

void Phone::addTaskToList(std::string toAdd)
{
	Tasks temptask(toAdd);
	taskList.push_back(temptask);
}

void Phone::markTaskAsCompleted(std::string taskToMark)
{
	for (unsigned i = 0; i < taskList.size(); i++) {
		if (taskList[i].taskName == taskToMark) {
			taskList[i].isCompleted = true;
		}
	}
}
