//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "UnitSpawnMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"


UnitSpawnMessage::UnitSpawnMessage(GameApp* game)
	:GameMessage(SPAWN_UNITS_MESSAGE)
{
	mpGameApp = game;
}


UnitSpawnMessage::~UnitSpawnMessage()
{
}

void UnitSpawnMessage::process()
{
	UnitManager* manager = mpGameApp->getUnitManager();

	while (manager->getMapSize() > 0)
	{
		manager->deleteRecentUnit();
	}

	for (int i = 0; i < 10; ++i)
	{
		manager->createRandomUnit(*mpGameApp->getSpriteManager()->getSprite(1));
	}
}
