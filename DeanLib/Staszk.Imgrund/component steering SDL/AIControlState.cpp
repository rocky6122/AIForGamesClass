#include "AIControlState.h"
#include "UnitManager.h"
#include "Game.h"
#include "SpriteManager.h"

void AIControlState::onEntrance()
{
	printf("Enter AIControl State\n");
	Unit* pUnit = gpGame->getUnitManager()->getPlayerUnit();

	pUnit->setSprite(*gpGame->getSpriteManager()->getSprite(4));

	// SET STEERING HERE
	pUnit->setSteering(Steering::ARRIVEANDFACE, Vector2D(-200,-200), INVALID_UNIT_ID);
}

void AIControlState::onExit()
{
	printf("Enter AIControl State\n");

	Unit* pUnit = gpGame->getUnitManager()->getPlayerUnit();

	pUnit->setSteering(Steering::INVALID_TYPE);

	PhysicsData data(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), 0.0f, 0.0f);
	pUnit->getPhysicsComponent()->setData(data);
}

StateTransition * AIControlState::update()
{
	Unit* pUnit = gpGame->getUnitManager()->getPlayerUnit();

	dealDamage(pUnit);

	Vector2D distanceVector;
	int IDofClosest = 0;
	float smallestDistance = INFINITY;
	Vector2D closestDistanceVector;

	float distanceActual;

	std::map<UnitID, Unit*> pUnitMap;

	pUnitMap = gpGame->getUnitManager()->getMap();

	for (auto it = pUnitMap.begin(); it != pUnitMap.end(); ++it)
	{
		if (it->first == PLAYER_UNIT_ID)
		{
			continue;
		}

		distanceVector = pUnit->getPositionComponent()->getPosition() - it->second->getPositionComponent()->getPosition();
		distanceActual = distanceVector.getLength();

		if (distanceActual < smallestDistance && distanceActual < mRADIUS * 3)
		{
			IDofClosest = it->first;
			smallestDistance = distanceActual;

			closestDistanceVector = distanceVector;
		}
	}

	if (IDofClosest != 0)
	{
		Vector2D newTarget(pUnit->getPositionComponent()->getPosition() + closestDistanceVector);
		pUnit->setSteering(Steering::ARRIVEANDFACE, newTarget, INVALID_UNIT_ID);
	}


	if (pUnit->isDead())
	{
		//GameOver
		gpGame->mShouldExit = true;
	}
	else if (!gpGame->mIsAIControlled)
	{
		auto iter = mTransitions.find(TO_ARRIVE_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;
}

void AIControlState::dealDamage(Unit* pUnit)
{
	Vector2D distanceVector;

	float distanceActual;

	std::map<UnitID, Unit*> pUnitMap;

	pUnitMap = gpGame->getUnitManager()->getMap();

	for (auto it = pUnitMap.begin(); it != pUnitMap.end(); ++it)
	{
		if (it->first == PLAYER_UNIT_ID)
		{
			continue;
		}

		distanceVector = pUnit->getPositionComponent()->getPosition() - it->second->getPositionComponent()->getPosition();
		distanceActual = distanceVector.getLength();

		if (distanceActual <= mRADIUS)
		{
			it->second->modifyHealth(pUnit->getDamageValue());
		}
	}
}
