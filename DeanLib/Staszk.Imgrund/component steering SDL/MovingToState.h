#pragma once

#include "StateMachine.h"
#include "Unit.h"

class MovingToState : public StateMachineState
{
public:
	MovingToState(const SM_idType& id, float radius) :StateMachineState(id), mRADIUS(radius) {};

	void onEntrance();
	void onExit();
	StateTransition* update();

	void dealDamage(Unit* pUnit);

private:
	float mRADIUS;
};