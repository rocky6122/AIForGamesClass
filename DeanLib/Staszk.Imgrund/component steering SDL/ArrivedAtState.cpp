#include "ArrivedAtState.h"
#include "game.h"
#include "UnitManager.h"
#include "SpriteManager.h"

void ArrivedAtState::onEntrance()
{
	printf("Enter ArrivedAt State\n");

	Unit* pUnit = gpGame->getUnitManager()->getPlayerUnit();

	pUnit->setSprite(*gpGame->getSpriteManager()->getSprite(3));

	pUnit->setSteering(Steering::ARRIVEANDFACE, Vector2D(pUnit->getPositionComponent()->getPosition()));
}

void ArrivedAtState::onExit()
{
	printf("Leaving ArrivedAt State\n");
}

StateTransition * ArrivedAtState::update()
{
	float tempVel;

	tempVel = gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getVelocity().getLength();

	dealDamage(gpGame->getUnitManager()->getPlayerUnit());

	if (gpGame->getUnitManager()->getPlayerUnit()->isDead()) //If dead, game over
	{
		//GAME OVER
		gpGame->mShouldExit = true;
	}
	else if (tempVel > 1.0f) //If moving again, moveTo
	{
		auto iter = mTransitions.find(TO_MOVE_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (gpGame->mIsAIControlled) //If AI is now in charge
	{
		auto iter = mTransitions.find(TO_AI_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;
}

void ArrivedAtState::dealDamage(Unit* pUnit)
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