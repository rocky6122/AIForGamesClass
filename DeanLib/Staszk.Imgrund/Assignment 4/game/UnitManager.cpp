//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include <map>
#include <DeanLibDefines.h>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "Defines.h"
#include "GameApp.h"
#include "Grid.h"

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
	mNextUnitID = 0;
}

Unit* UnitManager::createUnit(const Sprite& sprite, Vector2D position, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite, position);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = mNextUnitID;
			mNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;
	}

	return pUnit;
}

Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	
	int posX, posY;

	do 
	{
		posX = rand() % pGame->getGrid()->getGridWidth();
		posY = rand() % pGame->getGrid()->getGridHeight();

	} while (pGame->getGrid()->getValueAtPixelXY(posX * 32, posY * 32) == BLOCKING_VALUE);


	Unit* pUnit = createUnit(sprite, Vector2D(posX * 32, posY * 32), INVALID_UNIT_ID);

	if (pUnit != NULL)
	{
		//COULD BE USEFUL WHEN REVIEWING PATHFINDING GENERATION
	}
	return pUnit;
}

void UnitManager::setUnitPathfinding(const Vector2D & toVector)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->getAgent()->beginPathing(toVector);
	}
}

void UnitManager::finishPathfinding()
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->getAgent()->finishPathfinding();
	}
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}


//Deletes the last unit created on the map. Modified by Parker Staszkiewicz & John Imgrund
void UnitManager::deleteRecentUnit()
{	
	if (mNextUnitID > 0)
	{
		deleteUnit(mNextUnitID - 1);
		mNextUnitID--;

		mNextUnitID = (mNextUnitID < 0) ? 0 : mNextUnitID;
	}
}
