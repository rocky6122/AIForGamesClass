//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "WanderSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	:FaceSteering(ownerID, targetLoc, targetID)
{
	mType = Steering::WANDER;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	wanderRotation = pOwner->getFacing();
}

Steering* WanderSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	float randomVar = genRandomBinomial();
	//Sets a random wander direction
	wanderRotation += randomVar * WANDER_RATE;

	//calculate combined target orientation
	float targetRotation = wanderRotation + (pOwner->getFacing());

	//Calculates the center of the wander circle
	Vector2D targetVec = pOwner->getPositionComponent()->getPosition() + Vector2D(sin(pOwner->getFacing()), cos(pOwner->getFacing())) * WANDER_OFFSET;

	
	//Calculate the target location
	targetVec += Vector2D(sin(targetRotation), cos(targetRotation)) * WANDER_RADIUS;
	
	setTargetLoc(targetVec);
	

	Vector2D diff;
	
	diff = targetVec - pOwner->getPositionComponent()->getPosition();

	diff.normalize();

	//diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff * pOwner->getMaxAcc(); //- pOwner->getPositionComponent()->getPosition();
						  //data.vel.normalize();
	//data.vel *= 50;

	//data.acc = Vector2D(cos(pOwner->getFacing()), sin(pOwner->getFacing())) * MAX_ACC;

	this->mData = data;
	return FaceSteering::getSteering();;
}