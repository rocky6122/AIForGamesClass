//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "PositionComponent.h"
#include "Align.h"



FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Align(ownerID, targetLoc, targetID)
{
	mType = Steering::FACE;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* FaceSteering::getSteering()
{
	if (mTargetID != INVALID_UNIT_ID)
	{
		// Holder of static data for target
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		//assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	// Holder of static data for character
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (direction.getLength() == 0)
	{
		return this;
	}

	//Align* newAlignSteering;

	//newAlignSteering = new Align(mOwnerID, mTargetLoc, mTargetID);
	//newAlignSteering->setFacing(atan2(direction.getX(), -direction.getY()) + (6.28));

	Align::mManualFacing = atan2(direction.getX(), -direction.getY());

	return Align::getSteering();
}


