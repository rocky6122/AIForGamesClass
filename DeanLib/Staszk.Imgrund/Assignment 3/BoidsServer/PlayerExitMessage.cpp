//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//�We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.�

#include "GameMessageManager.h"
#include "PlayerExitMessage.h"
#include "Game.h"

//Assigns the bool to the member variable for shipping to another class
PlayerExitMessage::PlayerExitMessage(bool shouldExit) :GameMessage(PLAYER_EXIT_MESSAGE)
{
	mShouldExit = shouldExit;
}

PlayerExitMessage::~PlayerExitMessage()
{
}

void PlayerExitMessage::process()
{
	gpGame->mShouldExit = true;
}
