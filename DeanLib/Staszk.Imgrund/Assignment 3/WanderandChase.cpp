//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "WanderandChase.h"
#include "Unit.h"
#include "UnitManager.h"
#include "Game.h"



WanderandChase::WanderandChase(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID)
	:WanderSteering(ownerID, targetLoc, targetID)
{
	mType = Steering::WANDERANDCHASE;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	setTrueID(targetID);
}


Steering * WanderandChase::getSteering()
{
	//Only can chase against the player character
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	Vector2D distance = pPlayer->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();

	if (distance.getLength() < CHASE_RADIUS)
	{
		setTargetLoc(pPlayer->getPositionComponent()->getPosition());
		distance.normalize();
		distance *= pOwner->getMaxAcc();

		PhysicsData data = pOwner->getPhysicsComponent()->getData();
		data.acc = distance;
		//data.rotVel = 1.0f;
		this->mData = data;
		return FaceSteering::getSteering();
	}


	return WanderSteering::getSteering();
}
