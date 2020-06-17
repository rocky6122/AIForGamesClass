#pragma once
#include "StateMachine.h"
#include "UnitManager.h"

class SeekPickupState : public StateMachineState
{
public:
	SeekPickupState(const SM_idType& id, Unit* pUnit, float speedBoost, float damageBoost, float healthBoost) 
		:StateMachineState(id), mpUnit(pUnit), mSpeedBoost(speedBoost), mDamageBoost(damageBoost), mHealthBoost(healthBoost) {};

	void onEntrance();
	void onExit();
	StateTransition* update();

private: 
	Unit* mpUnit;
	float mPickupRadius = 20.0f; //Cant be data driven as this number is a direct result of the accuracy of our arrive steering threshold for success
	float mSpeedBoost;
	float mDamageBoost;
	float mHealthBoost;
	float mExitRadius = 100.0f;
};

