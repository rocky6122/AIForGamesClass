//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "UnitAdditionMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"
#include "Game.h"

UnitAdditionMessage::UnitAdditionMessage():GameMessage(UNIT_ADDITION_MESSAGE)
{
}


UnitAdditionMessage::~UnitAdditionMessage()
{
}

void UnitAdditionMessage::process()
{
	//THIS IS CALLED MULTIPLE TIMES INSIDE INPUTSYSTEM AS PER THE 10 UNIT PER A PRESS REQUIREMENT OF ASSIGNMENT 3 - JOHN IMGRUND

	Unit* pRandomUnit = gpGame->getUnitManager()->createRandomUnit(*gpGame->getSpriteManager()->getSprite(2), true);

	//Used to debug wander
	//pRandomUnit->setShowTarget(true);

	pRandomUnit->setSteering(Steering::FLOCKING, Vector2D(-100, -100), 0);
}