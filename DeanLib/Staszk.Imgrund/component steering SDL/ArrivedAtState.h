#pragma once

#include "StateMachine.h"
#include "Unit.h"

class ArrivedAtState : public StateMachineState
{
public:
	ArrivedAtState(const SM_idType& id, float radius) :StateMachineState(id), mRADIUS(radius) {};

	void onEntrance();
	void onExit();
	StateTransition* update();

	void dealDamage(Unit* pUnit);

private:
	float mRADIUS;
};