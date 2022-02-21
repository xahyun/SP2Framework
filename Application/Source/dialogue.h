#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>
#include <iostream>

struct dialogue
{
	std::string dialog;
	int rejectionDialogue;
	int confirmationDialogue;
};

class Chats {
public:
	std::vector<dialogue> chats;
	std::string speaker;
	std::vector<int> chatMarker;
	
	Chats();
	~Chats();

	void addDialogue(std::string dialog, int rejectionDialogue, int confirmationDialogue);
};

#endif