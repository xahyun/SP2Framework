#include "dialogue.h"

Chats::Chats()
{
	speaker = "Unknown";
}

Chats::~Chats()
{
	
}

void Chats::addDialogue(std::string dialog, int rejectionDialogue, int confirmationDialogue)
{
	dialogue Dialog;
	Dialog.dialog = dialog; 
	Dialog.rejectionDialogue = rejectionDialogue;
	Dialog.confirmationDialogue = confirmationDialogue;
	chats.push_back(Dialog);
}
