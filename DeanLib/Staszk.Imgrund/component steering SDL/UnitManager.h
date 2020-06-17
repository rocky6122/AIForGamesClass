#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"
#include "Timer.h"
#include <list>
#include <string>
#include <iostream>

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;

class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager();

	//Creation
	Unit* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID, const int unitType = 0);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Unit* createRandomUnit(const Sprite& sprite, const int unitType = 0);
	void initializeStateMachineTransitions();

	//Destruction
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();
	void deleteRecentUnit();

	//Modify
	void setInitialEnemyUnitState(Unit* pUnit);
	void setIntitalPlayerUnitState();

	void updateAll(float elapsedTime);
	void updatePickUps(float elapsedTime);
	void drawAll() const;

	//Gets
	Unit* getUnit(const UnitID& id) const;
	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };
	inline int getMapSize() { return mUnitMap.size(); }
	std::map<UnitID, Unit*> getMap() const { return mUnitMap; }

private:
	UnitID msNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;
	std::list<UnitID> mListToDelete;
	Timer* mpSpawnTimer;
	const float PICKUP_SPAWN_OFFSET = 3.0f;

	//Data Driven Values

	//Keywords in File

	//Player Keywords
	const std::string PLAYER_HEALTH_KEYWORD = "PlayerHealth";
	const std::string PLAYER_MOVEMENT_KEYWORD = "PlayerMovement";
	const std::string PLAYER_DAMAGE_KEYWORD = "PlayerDamage";
	const std::string PLAYER_RADIUS_KEYWORD = "PlayerRadius";

	//Enemy Keywords
	const std::string ENEMY_HEALTH_KEYWORD = "EnemyHealth";
	const std::string ENEMY_DAMAGE_KEYWORD = "EnemyDamage";
	const std::string ENEMY_RADIUS_KEYWORD = "EnemyRadius";
	const std::string ENEMY_SPAWN_RATE_KEYWORD = "EnemySpawnRate";

	//PickUp keywords
	const std::string SPEED_BOOST_KEYWORD = "EnemySpeedBoost";
	const std::string DAMAGE_BOOST_KEYWORD = "EnemyDamageBoost";
	const std::string ENEMY_HEAL_KEYWORD = "EnemyHeal";

	//Player Data
	float mPlayerHealth;
	float mPlayerMovement;
	float mPlayerDamage;
	float mPlayerRadius;

	//Enemy data
	float mEnemyHealth;
	float mEnemyDamage;
	float mEnemyRadius;
	float mEnemySpawnRate;

	//PickUp Data
	float mPickUpSpeedBoost;
	float mPickUpDamageBoost;
	float mPickUpHealthBoost;

	//Machine States

	//Player States
	StateMachineState* mppMovingToState; //'member pointer player'
	StateMachineState* mppArrivedAtState;
	StateMachineState* mppAIControlState;

	//Transitions

	//Player Transitions
	StateTransition* mppToMoveTransition; //'member pointer player'
	StateTransition* mppToArriveTransition;
	StateTransition* mppToAIControlTransition;

	//Enemy Transitions
	StateTransition* mpeToWanderTransition; //'member pointer enemy'
	StateTransition* mpeToDamageEnemyTransition;
	StateTransition* mpeToDieTransition;
	StateTransition* mpeToSeekPickUpTransition;
};