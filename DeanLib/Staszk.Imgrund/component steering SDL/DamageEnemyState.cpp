#include "DamageEnemyState.h"
#include "Game.h"
#include "UnitManager.h"
#include "SpriteManager.h"


void DamageEnemyState::onEntrance()
{
	printf("Enter Damage Enemy State\n");
	mpUnit->setSprite(*gpGame->getSpriteManager()->getSprite(6));
}

void DamageEnemyState::onExit()
{
	printf("Leaving Damage Enemy State\n");
}

StateTransition * DamageEnemyState::update()
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
	else if (mDistToPlayer > mRADIUS) //If the player is to far away, wander
	{
		auto iter = mTransitions.find(TO_WANDER_TRANSITION);

		if (iter != mTransitions.end())
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else //Damage the player
	{
		player->modifyHealth(mpUnit->getDamageValue());
	}

	return NULL;
}
