//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-405-01
//Project 2: Steering Behaviors
//10/26/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "ObjectPlacementMessage.h"
#include "UnitManager.h"
#include "Game.h"

ObjectPlacementMessage::ObjectPlacementMessage(Vector2D pos) : GameMessage(OBJECT_PLACEMENT_MESSAGE)
{
	mPos = pos;
}


ObjectPlacementMessage::~ObjectPlacementMessage()
{

}

void ObjectPlacementMessage::process()
{
	Unit* pRandomObstacleUnit = gpGame->getObstacleManager()->createUnit(true, PositionData(mPos, 0), ZERO_PHYSICS_DATA, INVALID_UNIT_ID, true);

	pRandomObstacleUnit->setSteering(Steering::INVALID_TYPE);
}
