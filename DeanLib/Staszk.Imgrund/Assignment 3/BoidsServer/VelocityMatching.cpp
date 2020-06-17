//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/28/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "VelocityMatching.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"



VelocityMatching::VelocityMatching(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	:Align(ownerID, targetLoc, targetID)
{
	mType = Steering::VELOCITYMATCHING;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

PhysicsData VelocityMatching::getPhysicsData()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* target = gpGame->getUnitManager()->getUnit(mTargetID);

	PhysicsData data;
	PhysicsData alignData;

	//Acceleration tries to get to the target velocity
	data.acc = target->getPhysicsComponent()->getVelocity() - pOwner->getPhysicsComponent()->getVelocity();
	data.acc /= TIME_TO_TARGET;

	// Check if acceleration is too fast
	if (data.acc.getLength() > MAX_SPEED)
	{
		data.acc.normalize();
		data.acc *= MAX_SPEED;
	}

	Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	Align::mManualFacing = atan2(direction.getX(), -direction.getY());


	data.rotAcc = 0;
	data.rotVel = 0;

	//Gets the rotation vel and rotation acc
	alignData = Align::getPhysicsData();

	data.rotVel = alignData.rotVel;
	data.rotAcc = alignData.rotAcc; //Might not need this

	return data;
}

Steering* VelocityMatching::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* target = gpGame->getUnitManager()->getUnit(mTargetID);

	PhysicsData data;

	//Acceleration tries to get to the target velocity
	data.acc = target->getPhysicsComponent()->getVelocity() - pOwner->getPhysicsComponent()->getVelocity();
	data.acc /= TIME_TO_TARGET;

	// Check if acceleration is too fast
	if (data.acc.getLength() > MAX_SPEED)
	{
		data.acc.normalize();
		data.acc *= MAX_SPEED;
	}

	Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	Align::mManualFacing = atan2(direction.getX(), -direction.getY());


	data.rotAcc = 0;
	data.rotVel = 0;
	this->mData = data;
	return Align::getSteering();
}