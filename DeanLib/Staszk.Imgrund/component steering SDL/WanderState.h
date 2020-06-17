#pragma once

#include "StateMachine.h"
#include "Unit.h"

class WanderState : public StateMachineState
{
public:
	WanderState(const SM_idType& id, Unit* pUnit, float radius):StateMachineState(id), mpUnit(pUnit), mRADIUS(radius){};

	void onEntrance();
	void onExit();
	StateTransition* update();

private:
	Unit* mpUnit;
	float mRADIUS;
	float mDistToPlayer = INFINITY;
};