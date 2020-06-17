//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/28/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "Flocking.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"

Flocking::Flocking(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, const float cohesionWeight, const float separationWeight, const float velocityMatchWeight)
	:Steering()
{
	mType = Steering::FLOCKING;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	cohesionObject = new Cohesion(ownerID, targetLoc, targetID);
	separationObject = new Separation(ownerID, targetLoc, targetID);
	velocityMatchingObject = new VelocityMatching(ownerID, targetLoc, targetID);

	mCohesionWeight = cohesionWeight;
	mSeparationWeight = separationWeight;
	mVelocityMatchingWeight = velocityMatchWeight;
}

Flocking::~Flocking()
{
	delete cohesionObject;
	delete separationObject;
	delete velocityMatchingObject;
}

void Flocking::updateValues(float c, float s, float v)
{
	mCohesionWeight = c;
	mSeparationWeight = s;
	mVelocityMatchingWeight = v;
}

Steering * Flocking::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData flockingData = pOwner->getPhysicsComponent()->getData();

	PhysicsData cohesionData;
	PhysicsData separationData;
	PhysicsData velocityMatchingData;

	//Gets Cohesion Data
	cohesionData = cohesionObject->getPhysicsData();
	flockingData.acc += cohesionData.acc * mCohesionWeight;

	//Gets Separation Data
	separationData = separationObject->getPhysicsData();
	flockingData.acc += separationData.acc * mSeparationWeight;
	
	//Gets Velocity Matching Data
	velocityMatchingData = velocityMatchingObject->getPhysicsData();
	flockingData.rotVel = velocityMatchingData.rotVel * mVelocityMatchingWeight;
	flockingData.rotAcc = velocityMatchingData.rotAcc * mVelocityMatchingWeight; //Might not need this;

	//mFinalVec = COHESION_WEIGHT * cohesionObject.getData();
	//mFinalVec = SEPARATION_WEIGHT * separationObject.getVector();
	//mFinalVec = VELOCITY_MATCHING_WEIGHT * velocityMatchingObject.getVector();


	this->mData = flockingData;
	return this;
}
