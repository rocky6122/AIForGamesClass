//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/25/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "Cohesion.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"

Cohesion::Cohesion(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID)
	:SeekSteering(ownerID, targetLoc, targetID)
{
	mType = COHESION;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

PhysicsData Cohesion::getPhysicsData()
{
	//Creates floats for our next Center Position
	float x = 0;
	float y = 0;
	int count = 0;

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* temp;
	Vector2D direction;
	float distance;

	int numOfBoids = (gpGame->getNetworkInfo()->model == DATA_SHARING) ? LOCAL_BOID_NUM + 1 : gpGame->getUnitManager()->getMapSize();

	for (int i = 1; i < numOfBoids; i++)
	{
		if (i == pOwner->getUnitID())
		{
			//Dont want to check against ourselves
		}
		else
		{
			//Selects new Target
			temp = gpGame->getUnitManager()->getUnit(i);

			if (temp != NULL)
			{
				//Check if the target is close enough
				direction = pOwner->getPositionComponent()->getPosition() - temp->getPositionComponent()->getPosition();
				distance = direction.getLength();

				//If that target is within the threshold distance move away
				if (distance < RADIUS)
				{
					x += gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getX();
					y += gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getY();
					count++;
				}
			}
		}
	}

	//Gets the average X and Y values of all Units
	x /= count;
	y /= count;

	Vector2D diffToTarget;
	Vector2D averageLoc;

	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		//assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
		diffToTarget = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}


	averageLoc = Vector2D(x, y) - pOwner->getPositionComponent()->getPosition();

	diffToTarget.normalize();
	diffToTarget *= pOwner->getMaxAcc();

	averageLoc.normalize();

	Vector2D blendedData = averageLoc * 20.0f + diffToTarget;
	blendedData.setX(blendedData.getX() / 2.0f);
	blendedData.setY(blendedData.getY() / 2.0f);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = blendedData;

	//data.acc = averageLoc;

	return data;
}

Steering* Cohesion::getSteering()
{
	//Creates floats for our next Center Position
	float x = 0;
	float y = 0;
	int count = 0;

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Unit* temp;
	Vector2D direction;
	float distance;

	for (int i = 1; i < gpGame->getUnitManager()->getMapSize(); i++)
	{
		if (i == pOwner->getUnitID())
		{
			//Dont want to check against ourselves
		}
		else
		{
			//Selects new Target
			temp = gpGame->getUnitManager()->getUnit(i);

			if (temp != NULL)
			{
				//Check if the target is close enough
				direction = pOwner->getPositionComponent()->getPosition() - temp->getPositionComponent()->getPosition();
				distance = direction.getLength();

				//If that target is within the threshold distance move away
				if (distance < RADIUS)
				{
					x += gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getX();
					y += gpGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getY();
					count++;
				}
			}
		}
	}

	//Gets the average X and Y values of all Units
	x /= count;
	y /= count;


	SeekSteering::mType = Steering::SEEK; //COULD BE FUCKED
	SeekSteering::setTargetLoc(Vector2D(x, y));

	return SeekSteering::getSteering();
}
