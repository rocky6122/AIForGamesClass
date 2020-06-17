//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "UnitDeletionMessage.h"
#include "Game.h"
#include "UnitManager.h"

UnitDeletionMessage::UnitDeletionMessage() : GameMessage(UNIT_DELETION_MESSAGE)
{
}


UnitDeletionMessage::~UnitDeletionMessage()
{
}

void UnitDeletionMessage::process()
{
	//This function was already written and works as intended by the project parameters.
	gpGame->getUnitManager()->deleteRandomUnit();
}