//Team Member Names: John Imgrund (82-1019344) Parker Staszkiewicz (82-0967451)
//EGP-410-01
//Assignment 5: Advanced Pathfinding
//10/31/18
//“We certify that this work is
//entirely our own.The assessor of this project may reproduce this project
//and provide copies to other academic staff, and/or communicate a copy of
//this project to a plagiarism - checking service, which may retain a copy of the
//project on its database.”

#pragma once
#include "Vector2D.h"
#include "Path.h"

class NavAgent
{
public:
	NavAgent(Vector2D location);
	~NavAgent();

	void update(float elapsedTime);
	bool moveTowards(const Vector2D& toVector, float delta);

	void setPath(const Vector2D& toVector);
	void beginPathing(const Vector2D& toVector);
	void finishPathfinding();

	inline Vector2D getPosition() const { return mCurrentLocation; }

private:
	Path* mpPath;
	int mCurrentIndex;
	float mSpeed;

	Vector2D mCurrentLocation;

	bool mIsPathing;
};

