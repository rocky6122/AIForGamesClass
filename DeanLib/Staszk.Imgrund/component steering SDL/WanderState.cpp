#include "WanderState.h"
#include "UnitManager.h"
#include "Game.h"
#include "SpriteManager.h"

void WanderState::onEntrance()
{
	printf("Enter Wander State\n");
	mpUnit->setSteering(Steering::WANDER, ZERO_VECTOR2D, INVALID_UNIT_ID);
	mpUnit->setSprite(*gpGame->getSpriteManager()->getSprite(5));
}

void WanderState::onExit()
{
	printf("Leaving Wander State\n");
	/*mpUnit->setSteering(Steering::INVALID_TYPE);

	PhysicsData data(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), 0.0f, 0.0f);
	mpUnit->getPhysicsComponent()->setData(data);*/
}

StateTransition * WanderState::update()
{
	Unit* player = gpGame->getUnitManager()->getPlayerUnit();

	Vector2D dist = mpUnit->getPositionComponent()->getPosition() - player->getPositionComponent()->getPosition();
	mDistToPlayer = dist.getLength();

	if (mpUnit->isDead()) //If dead, die
	{
		auto iter = mTransitions.find(TO_DIE_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (mDistToPlayer <= mRADIUS) //If in radius of player, do damage
	{
		auto iter = mTransitions.find(TO_DAMAGE_ENEMY_TRANSITION);
	
		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else if (gpGame->getPickupManager()->getMapSize() > 0) //if a pickUp exists, see if its close.
	{
		Unit* pickup = gpGame->getPickupManager()->getUnit(1);

		if (pickup)
		{
			dist = mpUnit->getPositionComponent()->getPosition() - pickup->getPositionComponent()->getPosition();
			float distance = dist.getLength();

			if (distance < mRADIUS) //If its close, seek it
			{
				auto iter = mTransitions.find(TO_SEEK_PICKUP_TRANSITION);

				if (iter != mTransitions.end())
				{
					StateTransition* pTransition = iter->second;
					return pTransition;
				}
			}
		}
	}

	return NULL; //No transition
}
