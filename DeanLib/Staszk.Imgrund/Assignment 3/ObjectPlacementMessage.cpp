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
#include "SpriteManager.h"
#include "Game.h"
#include "Networking.h"


ObjectPlacementMessage::ObjectPlacementMessage(Vector2D pos, bool isNetworked) : GameMessage(OBJECT_PLACEMENT_MESSAGE)
{
	mPos = pos;
	mIsNetworked = isNetworked;
}


ObjectPlacementMessage::~ObjectPlacementMessage()
{

}

void ObjectPlacementMessage::process()
{
	if (mIsNetworked)
	{
		Unit* pRandomObstacleUnit = gpGame->getObstacleManager()->createUnit(*gpGame->getSpriteManager()->getSprite(4), true, PositionData(mPos, 0), ZERO_PHYSICS_DATA, INVALID_UNIT_ID, false);

		pRandomObstacleUnit->setSteering(Steering::INVALID_TYPE);
	}
	else
	{

		switch (gpGame->getNetworkInfo()->model)
		{
		case DATA_PUSH:
		{
			ObstacleEventMessage msg[1];

			msg->obstaclePos = mPos;
			gpGame->getNetworkInfo()->peer->Send((char*)(msg), sizeof(ObstacleEventMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->host_id, false);
		}
		break;

		case DATA_SHARING:
		{
			Unit* pRandomObstacleUnit = gpGame->getObstacleManager()->createUnit(*gpGame->getSpriteManager()->getSprite(4), true, PositionData(mPos, 0), ZERO_PHYSICS_DATA, INVALID_UNIT_ID, true);

			pRandomObstacleUnit->setSteering(Steering::INVALID_TYPE);

			ObstacleEventMessage msg[1];

			msg->obstaclePos = mPos;
			gpGame->getNetworkInfo()->peer->Send((char*)(msg), sizeof(ObstacleEventMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->host_id, false);
		}
		break;

		case DATA_COUPLING:
		{
			Unit* pRandomObstacleUnit = gpGame->getObstacleManager()->createUnit(*gpGame->getSpriteManager()->getSprite(4), true, PositionData(mPos, 0), ZERO_PHYSICS_DATA, INVALID_UNIT_ID, true);

			pRandomObstacleUnit->setSteering(Steering::INVALID_TYPE);

			ObstacleEventMessage msg[1];

			msg->obstaclePos = mPos;
			gpGame->getNetworkInfo()->peer->Send((char*)(msg), sizeof(ObstacleEventMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->getNetworkInfo()->host_id, false);
		}
		break;
		}
	}
}
