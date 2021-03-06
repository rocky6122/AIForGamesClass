//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 3: Flocking
//9/28/18
//�We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.�

#include "FlockingCohesionWeightMessage.h"

FlockingCohesionWeightMessage::FlockingCohesionWeightMessage(const float delta)
	:GameMessage(FLOCKING_WEIGHT_COHESION)
{
	mDelta = delta;
}

FlockingCohesionWeightMessage::~FlockingCohesionWeightMessage()
{
}

void FlockingCohesionWeightMessage::process()
{
	gpGame->cohesionWeight += mDelta;
	gpGame->updateFlock();
}
