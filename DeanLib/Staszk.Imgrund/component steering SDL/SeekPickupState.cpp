#include "SeekPickupState.h"
#include "Game.h"
#include "SpriteManager.h"

void SeekPickupState::onEntrance()
{
	printf("Enter Seek Pickup State\n");
	mpUnit->setSprite(*gpGame->getSpriteManager()->getSprite(7));

	mpUnit->setSteering(Steering::ARRIVEANDFACE, 
		gpGame->getPickupManager()->getUnit(1)->getPositionComponent()->getPosition(), 
		INVALID_UNIT_ID);
}

void SeekPickupState::onExit()
{
	printf("Leaving Seek Pickup State\n");
}

StateTransition * SeekPickupState::update()
{
	Unit* pickup = gpGame->getPickupManager()->getUnit(1);

	float distance = INFINITY;

	if (pickup)
	{
		Vector2D dist = mpUnit->getPositionComponent()->getPosition() - pickup->getPositionComponent()->getPosition();
		distance = dist.getLength();
	}


	if (mpUnit->isDead()) //If dead, die
	{
		auto iter = mTransitions.find(TO_DIE_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (gpGame->getPickupManager()->getMapSize() < 1) //If No PickUps, wander
	{
		auto iter = mTransitions.find(TO_WANDER_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (distance <= mPickupRadius) //If collecting PickUp, gain pickUp power then wander
	{
		// Add effect of pickup to this unit
		int pickupType = gpGame->getPickupManager()->getUnit(1)->getUnitType();

		switch (pickupType)
		{
		case 1: //Speed Boost
		{
			PhysicsData data(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), 0.0f, 0.0f, (220.0f * mSpeedBoost));
			mpUnit->getPhysicsComponent()->setData(data);
		}
		break;

		case 2: //Damage Boost
		{
			mpUnit->modDamageValue(mDamageBoost);
		}
		break;

		case 3: //Health Boost
		{
			mpUnit->modifyHealth(mHealthBoost);
		}
		break;
		}

		// Delete pickup from manager
		gpGame->getPickupManager()->deleteRecentUnit();

		auto iter = mTransitions.find(TO_WANDER_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (distance > mExitRadius)
	{
		auto iter = mTransitions.find(TO_WANDER_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	return NULL;
}
