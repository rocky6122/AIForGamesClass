//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 4: Pathfinding
//10/10/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "ChangePathfindingMessage.h"
#include "PathfinderManager.h"



ChangePathfindingMessage::ChangePathfindingMessage(PathfindingType type, GameApp* gameApp) : GameMessage(CHANGE_PATHFINDING_MESSAGE)
{
	mType = type;
	mGameApp = gameApp;
}


ChangePathfindingMessage::~ChangePathfindingMessage()
{
}

void ChangePathfindingMessage::process()
{
	mGameApp->getPathfinderManager()->setPathfinders(mType);
}
