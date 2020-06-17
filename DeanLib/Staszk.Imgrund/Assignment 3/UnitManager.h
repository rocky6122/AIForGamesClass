#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;


class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager(){};

	Unit* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID, bool local = true);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Unit* createRandomUnit(const Sprite& sprite, bool local);
	void addUnit(Unit* unitToAdd);

	UnitID getUnitNextID() { return mNextUnitID; };

	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();
	void deleteRecentUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	//CREATED BY JOHN IMGRUND FOR USE IN ASSIGNMENT 3
	//NEEDED TO PROPPERLY CIRCLE THROUGH ALL UNITS IN THE UNITMANAGER
	int getMapSize() const { return mUnitMap.size(); };

	std::map<UnitID, Unit*> getMap() { return mUnitMap; }

private:
	UnitID mNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;
};

