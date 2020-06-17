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

#include "Trackable.h"
#include "Steering.h"

class Separation : public Steering
{
public:
	Separation(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);

	//Returns data needed for flocking.
	PhysicsData getPhysicsData();

protected:
	Steering* getSteering();

private:
	//NONE OF THESE ARE CONST BECAUSE THEY ARE MEANT TO BE CHANGED BY THE USER
	//THESE ARE JUST DEFAULT VALUES

	float mThreshold = 60.0f; //VALUE AT WHICH THE BIRDS WILL BEGIN TO MOVE APART
	float mDecayCoefficient = 10000.0f; //DETERMINES HOW FAST THE SEPARATION STRENGTH DECAYS WITH DISTANCE
};