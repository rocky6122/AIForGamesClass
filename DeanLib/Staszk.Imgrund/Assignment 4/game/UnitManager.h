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
#include <MemoryPool.h>
#include <map>

#include "Unit.h"

class Unit;
class Sprite;


class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager(){};

	Unit* createUnit(const Sprite& sprite, Vector2D position, const UnitID& id = INVALID_UNIT_ID);

	Unit* createRandomUnit(const Sprite& sprite);

	UnitID getUnitNextID() { return mNextUnitID; };

	void setUnitPathfinding(const Vector2D& toVector);
	void finishPathfinding();

	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();
	void deleteRecentUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	//CREATED BY JOHN IMGRUND FOR USE IN ASSIGNMENT 3
	//NEEDED TO PROPPERLY CIRCLE THROUGH ALL UNITS IN THE UNITMANAGER
	int getMapSize() const { return mUnitMap.size(); };

	std::map<UnitID, Unit*> getMap() { return mUnitMap; }

private:
	UnitID mNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;
};

