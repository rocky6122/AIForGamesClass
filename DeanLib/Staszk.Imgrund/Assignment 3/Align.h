//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 2: Steering Behaviors
//9/17/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once
#include <Trackable.h>
#include "Steering.h"

class Align : public Steering
{
public:
	Align(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);

	PhysicsData getPhysicsData();
	

protected:
	float mManualFacing = 0.0f;
	virtual Steering* getSteering();

private:
	float map(float value, float minOne, float maxOne, float minTwo, float maxTwo);

	const float TARGET_RADIUS = 0.06f;
	const float SLOW_RADIUS = 0.7f;
	const float TIME_TO_TARGET = 0.1f;
	const float MAX_ROT_ACC = 30.0f;
};

