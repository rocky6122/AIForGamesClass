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
#include "FaceSteering.h"


class ArriveAndFaceSteering : public FaceSteering
{
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);
protected:
	Steering* getSteering();
private:
	const float TARGET_RADIUS = 20.0f;
	const float SLOW_RADIUS = 250.0f;
	const float TIME_TO_TARGET = 0.1f;
};

