//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "Align.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include <math.h>


Align::Align(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID)
	:Steering()
{
	mType = Steering::ALIGN;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

PhysicsData Align::getPhysicsData()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	float rotation;

	// data carries over from the previous iteration.
	PhysicsData data = this->mData;


	// FOR FACE STEERING
	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);

		// Get direction to target
		rotation = pTarget->getFacing() - pOwner->getFacing();
	}
	else
	{
		rotation = mManualFacing - pOwner->getFacing();
	}

	// map the rotation to (-pi, pi) interval
	//rotation = map(rotation, -6.28, 6.28, -3.14, 3.14);
	rotation = (float)remainder(rotation, 6.28);
	float rotationSize = abs(rotation);

	// check if we are are alligned correctly
	if (rotationSize < TARGET_RADIUS)
	{
		data.rotAcc = 0;
		data.rotVel = 0;

		return data;
	}

	// Amount we want to rotate this frame, in RADIANS
	float targetRotation;

	// If we are outside the slow radius then use max rotation
	if (rotationSize > SLOW_RADIUS)
	{
		targetRotation = MAX_ROT_ACC;
	}
	else
	{
		targetRotation = MAX_ROT_ACC * rotationSize / SLOW_RADIUS;
	}

	// resultant target rotation
	targetRotation *= rotation / rotationSize;

	// Accelerate to target rotation
	data.rotAcc = targetRotation - pOwner->getFacing();
	data.rotAcc /= TIME_TO_TARGET;

	// check if Acceleration is too great
	float angularAcc = abs(data.rotAcc);
	if (angularAcc > MAX_ROT_ACC)
	{
		data.rotAcc /= angularAcc;
		data.rotAcc *= MAX_ROT_ACC;
	}

	// Without this line, rotationSize is never within targetRadius
	// but it stops turning anyway
	data.rotVel += (0.05f) * data.rotAcc;

	// output steering
	return data;
}

//void Align::setFacing(float facing)
//{
//	mManualFacing = facing;
//}

Steering* Align::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	float rotation;

	// data carries over from the previous iteration.
	PhysicsData data = this->mData;


	// FOR FACE STEERING
	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);

		// Get direction to target
		rotation = pTarget->getFacing() - pOwner->getFacing();
	}
	else
	{
		rotation = mManualFacing - pOwner->getFacing();
	}

	// map the rotation to (-pi, pi) interval
	//rotation = map(rotation, -6.28, 6.28, -3.14, 3.14);
	rotation = (float)remainder(rotation, 6.28);
	float rotationSize = abs(rotation);

	// check if we are are alligned correctly
	if (rotationSize < TARGET_RADIUS)
	{
		data.rotAcc = 0;
		data.rotVel = 0;

		this->mData = data;
		return this;
	}

	// Amount we want to rotate this frame, in RADIANS
	float targetRotation;

	// If we are outside the slow radius then use max rotation
	if (rotationSize > SLOW_RADIUS)
	{
		targetRotation = MAX_ROT_ACC;
	}
	else
	{
		targetRotation = MAX_ROT_ACC * rotationSize / SLOW_RADIUS;
	}

	// resultant target rotation
	targetRotation *= rotation / rotationSize;

	// Accelerate to target rotation
	data.rotAcc = targetRotation - pOwner->getFacing();
	data.rotAcc /= TIME_TO_TARGET;

	// check if Acceleration is too great
	float angularAcc = abs(data.rotAcc);
	if (angularAcc > MAX_ROT_ACC)
	{
		data.rotAcc /= angularAcc;
		data.rotAcc *= MAX_ROT_ACC;
	}

	// Without this line, rotationSize is never within targetRadius
	// but it stops turning anyway
	data.rotVel += (0.05f) * data.rotAcc;

	// output steering
	this->mData = data;
	return this;
}

float Align::map(float value, float minOne, float maxOne, float minTwo, float maxTwo)
{
	float leftSpan = maxOne - minOne;
	float rightSpan = maxTwo - minTwo;

	float valueScaled = (value - minOne) / leftSpan;

	return minTwo + (valueScaled * rightSpan);
}
