#ifndef PHONE_H
#define PHONE_H

#include <string>
#include <vector>
#include "dialogue.h"
#include "tasks.h"
#include "MyMath.h"

class Phone {
private:
	float timer;
	int randomNumber;

public:
	std::vector<Tasks> taskList;
	std::vector<Chats> chats;
	std::vector<Chats> existingChats;

	Phone();
	~Phone();

	void Update(float dt);
	
	//other things??
	void GenerateRandomChats();
	void addTaskToList(std::string toAdd);
	void markTaskAsCompleted(std::string taskToMark);
};

#endif