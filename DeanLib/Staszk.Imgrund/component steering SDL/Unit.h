#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
#include "StateMachine.h"
//#include "CircularQueue.h"
//#include "Transaction.h"
//#include "TransactionProcessor.h"

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;


//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	void draw() const;
	float getFacing() const;
	void update();

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	UnitID getUnitID() const { return mID; };
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	inline void prepareToDelete() { mDirty = true; };
	inline bool isDirty() { return mDirty; };

	inline void setSprite(const Sprite& newSprite) { mSprite = newSprite; };

	inline StateMachine* getStateMachine() { return mpStateMachine; };

	void modifyHealth(int healthValue);
	float getHealthValue() const { return mCurrentHealth; };

	float getDamageValue() const { return mDamageVal; };
	void modDamageValue(float damageBoost) { mDamageVal *= damageBoost; };

	bool isDead() { return mCurrentHealth <= 0; };
	StateMachine* InitializeStateMachine();

	inline int getUnitType() { return mUnitType; };

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);

private:
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	PositionComponent* mpPositionComponent = NULL;
	Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	StateMachine* mpStateMachine;

	//Unit Info
	float mMaxHealth;
	float mCurrentHealth;
	float mDamageVal;
	float mDamageRadius;

	int mUnitType;

	bool mDirty;

	Unit(const Sprite& sprite, const int unitType = 0, const float maxHealth = 0, const float damageVal = 0, const float damageRadius = 0);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
