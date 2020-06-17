#pragma once
#include "StateMachine.h"
#include "Unit.h"

class DeathState : public StateMachineState
{
public:
	DeathState(const SM_idType& id, Unit* pUnit) :StateMachineState(id), mpUnit(pUnit){};

	void onEntrance();
	void onExit();
	StateTransition* update();

private:
	Unit* mpUnit;
};

