//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/25/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#include "Separation.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"


Separation::Separation(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID)
{
	mType = Steering::SEPARATION;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

PhysicsData Separation::getPhysicsData()
{
	//Variables needed for steering
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data;

	//Variables for calculating distance between units
	Unit* temp;
	Vector2D direction;
	float distance;

	//Calculates strength of repulsion
	float strength;

	int numOfBoids = (gpGame->getNetworkInfo()->model == DATA_SHARING) ? LOCAL_BOID_NUM + 1 : gpGame->getUnitManager()->getMapSize();


	for (int i = 1 /*i = 1 because 0 is reserved for playerUnit*/; i < numOfBoids; i++)
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
				if (distance < mThreshold)
				{
					//Calculate the strength of repulsion
					//Uses inverse square law
					//Book says to take the min value, but thats way to slow, so we are taking the max
					strength = max(mDecayCoefficient / (distance * distance), pOwner->getMaxAcc());

					direction.normalize();

					data.acc += direction * strength;
				}
			}
		}
	}

	for (int i = 1; i < gpGame->getObstacleManager()->getMapSize(); ++i)
	{
		//Selects new Target
		temp = gpGame->getObstacleManager()->getUnit(i);

		if (temp != NULL)
		{
			//Check if the target is close enough
			direction = pOwner->getPositionComponent()->getPosition() - temp->getPositionComponent()->getPosition();
			distance = direction.getLength();

			//If that target is within the threshold distance move away
			if (distance < mThreshold)
			{
				//Calculate the strength of repulsion
				//Uses inverse square law
				//Book says to take the min value, but thats way to slow, so we are taking the max
				strength = max(mDecayCoefficient / (distance * distance), pOwner->getMaxAcc());

				direction.normalize();

				data.acc += direction * strength;
			}
		}
	}

	return data;
}

Steering * Separation::getSteering()
{
	//Variables needed for steering
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data;

	//Variables for calculating distance between units
	Unit* temp;
	Vector2D direction;
	float distance;

	//Calculates strength of repulsion
	float strength;

	
	for (int i = 1 /*i = 1 because 0 is reserved for playerUnit*/; i < gpGame->getUnitManager()->getMapSize(); i++)
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
				if (distance < mThreshold)
				{
					//Calculate the strength of repulsion
					//Uses inverse square law
					//Book says to take the min value, but thats way to slow, so we are taking the max
					strength = max(mDecayCoefficient / (distance * distance), pOwner->getMaxAcc());

					direction.normalize();

					data.acc += direction * strength;
				}
			}
		}
	}

	this->mData = data;
	return this;
}
