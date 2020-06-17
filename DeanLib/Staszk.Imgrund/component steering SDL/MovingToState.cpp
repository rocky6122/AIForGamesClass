#include "MovingToState.h"
#include "Game.h"
#include "UnitManager.h"
#include "SpriteManager.h"

void MovingToState::onEntrance()
{
	printf("Enter MovingTo State\n");

	gpGame->getUnitManager()->getPlayerUnit()->setSprite(*gpGame->getSpriteManager()->getSprite(2));
}

void MovingToState::onExit()
{
	printf("Leaving MovingTo State\n");
}

StateTransition * MovingToState::update()
{
	float tempVel;

	tempVel = gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getVelocity().getLength();

	dealDamage(gpGame->getUnitManager()->getPlayerUnit());

	if (gpGame->getUnitManager()->getPlayerUnit()->isDead()) //If dead, game over
	{
		//GAME OVER
		gpGame->mShouldExit = true;
	}
	else if (tempVel < 1.0f) //Checks to see if the player is not moving
	{
		auto iter = mTransitions.find(TO_ARRIVE_TRANSITION);

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


	return NULL; //No Transition
}

void MovingToState::dealDamage(Unit* pUnit)
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