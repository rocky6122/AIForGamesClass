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

#include <Trackable.h>
#include <limits>
#include <Vector2D.h>

#include "Defines.h"
#include "Sprite.h"

#include "NavAgent.h"

class Sprite;
class UnitManager;

//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	void draw() const;

	void update(float elapsedTime);

	UnitID getUnitID() const { return mID; };

	void setShowTarget(bool val) { mShowTarget = val; };

	inline NavAgent* getAgent() const { return mNavAgent; }

private:
	UnitID mID;
	Sprite mSprite;
	Vector2D mPosition;

	NavAgent* mNavAgent;

	bool mShowTarget;

	Unit(const Sprite& sprite, Vector2D position);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
