//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”


#include <cassert>
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::ARRIVE;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering * ArriveSteering::getSteering()
{
	// Ensure that there is a target to arrive at
	if (mTargetID != INVALID_UNIT_ID)
	{
		// Holder of static data for target
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		//assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	Vector2D dir;
	float dist;
	PhysicsData data;

	// Holder of static data for character
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	data = pOwner->getPhysicsComponent()->getData();

	// Gets the velocity (direction) to the target
	dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	// Get the distance
	dist = dir.getLength();

	// Checks if character is within radius of target
	if (dist < TARGET_RADIUS)
	{
		// return no steering, because we have arrived
		data.acc = 0;
		data.vel = 0;
		this->mData = data;
		return this;
	}

	float targetSpeed;

	// Determine target speed based on relation to the slow-down radius
	if (dist > SLOW_RADIUS)
	{
		targetSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		targetSpeed = pOwner->getMaxSpeed() * dist / SLOW_RADIUS;
	}

	// Determine target velocity
	Vector2D targetVelocity = dir;
	dir.normalize();
	targetVelocity *= targetSpeed;

	// Determine target acceleration
	data.acc = targetVelocity - data.vel;
	data.acc /= TIME_TO_TARGET;

	// Check if acceleration is too fast
	if (data.acc.getLength() > data.maxAccMagnitude)
	{
		data.acc.normalize();
		data.acc *= data.maxAccMagnitude;
	}

	this->mData = data;
	return this;
}
