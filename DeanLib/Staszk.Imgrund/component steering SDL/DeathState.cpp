#include "DeathState.h"
#include "Game.h"
#include "UnitManager.h"

void DeathState::onEntrance()
{
	printf("Enter Death State\n");
	mpUnit->prepareToDelete();
}

void DeathState::onExit()
{
	printf("Leaving Death State\n");
}

StateTransition * DeathState::update()
{
	return NULL;
}
