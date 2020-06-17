//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/25/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once
#include "SeekSteering.h"

class Cohesion : public SeekSteering
{
public:
	Cohesion(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);

	//Used for flocking. Calculates the data for cohesion.
	PhysicsData getPhysicsData();

protected:
	Steering* getSteering();

private:
	const float RADIUS = 400.0f;
};

