//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "ExitMessage.h"
#include "GameApp.h"
#include "Game.h"
#include "UnitManager.h"


ExitMessage::ExitMessage() : GameMessage(EXIT_GAME_MESSAGE)
{
}


ExitMessage::~ExitMessage()
{
}

void ExitMessage::process()
{
	gpGame->markForExit();

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	pGame->getUnitManager()->finishPathfinding();
}
