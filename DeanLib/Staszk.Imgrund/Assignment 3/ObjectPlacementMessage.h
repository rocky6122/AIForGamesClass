//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-405-01
//Project 2: Steering Behaviors
//10/26/18
//�We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.�

#pragma once
#include "GameMessage.h"
#include <Vector2D.h>

class ObjectPlacementMessage : public GameMessage
{
public:
	ObjectPlacementMessage(Vector2D pos, bool isNetworked);
	~ObjectPlacementMessage();

	void process();

private:
	Vector2D mPos;
	bool mIsNetworked;
};

