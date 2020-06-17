//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/28/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "SaveLoadMessage.h"
#include "Game.h"



SaveLoadMessage::SaveLoadMessage(const bool isLoad)
	:GameMessage(GAME_SAVE_LOAD)
{
	mLoad = isLoad;
}

SaveLoadMessage::~SaveLoadMessage()
{
}

void SaveLoadMessage::process()
{
	if (mLoad)
	{
		// call game load function
		gpGame->loadWeights();
	}
	else
	{
		// call game save function
		gpGame->saveWeights();
	}
}
