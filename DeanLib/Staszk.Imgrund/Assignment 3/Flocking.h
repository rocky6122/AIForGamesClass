//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/28/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once
#include "Cohesion.h"
#include "Separation.h"
#include "VelocityMatching.h"


class Flocking : public Steering
{
public:
	Flocking(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, const float cohesionWeight, const float separationWeight, const float velocityMatchWeight);

	~Flocking();

	Cohesion* cohesionObject;
	Separation* separationObject;
	VelocityMatching* velocityMatchingObject;

	void updateValues(float c, float s, float v);

protected:
	Steering* getSteering();

private:
	Vector2D mFinalVec;
	float mCohesionWeight = 2.0f;
	float mSeparationWeight = 1.0f;
	float mVelocityMatchingWeight = 1.0f;
};

