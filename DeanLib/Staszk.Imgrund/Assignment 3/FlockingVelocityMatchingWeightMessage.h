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

#include "GameMessage.h"
#include "Game.h"

class FlockingVelocityMatchingWeightMessage : public GameMessage
{
public:
	FlockingVelocityMatchingWeightMessage(const float delta);
	~FlockingVelocityMatchingWeightMessage();

	void process();

private:
	float mDelta;
};

