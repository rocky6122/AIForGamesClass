#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "SpriteManager.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "StateMachine.h"
#include "StatesList.h"

#include <time.h>
#include <fstream>
#include <algorithm>

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit)),
	mpeToDamageEnemyTransition(NULL),
	mpeToDieTransition(NULL),
	mpeToSeekPickUpTransition(NULL),
	mpeToWanderTransition(NULL),
	mppToAIControlTransition(NULL),
	mppToArriveTransition(NULL),
	mppToMoveTransition(NULL),
	mpSpawnTimer(NULL)
{
	msNextUnitID = PLAYER_UNIT_ID + 1;

	//Read in values from textFile
	string newLine;

	mpSpawnTimer = new Timer();
	mpSpawnTimer->start();

	ifstream dataFile(gpGame->getDataDrivenFile());

	if (dataFile.is_open())
	{
		while (getline(dataFile, newLine))
		{
			if (newLine == PLAYER_HEALTH_KEYWORD) //START OF PLAYER INFO DDD
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPlayerHealth = stof(toConvert);
			}
			else if (newLine == PLAYER_MOVEMENT_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPlayerMovement = stof(toConvert);
			}
			else if (newLine == PLAYER_DAMAGE_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPlayerDamage = stof(toConvert);

				mPlayerDamage = -mPlayerDamage;
			}
			else if (newLine == PLAYER_RADIUS_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPlayerRadius = stof(toConvert);
			}
			else if (newLine == ENEMY_HEALTH_KEYWORD) //START OF ENEMY INFO DDD
			{
				string toConvert;
				getline(dataFile, toConvert);

				mEnemyHealth = stof(toConvert);
			}
			else if (newLine == ENEMY_DAMAGE_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mEnemyDamage = stof(toConvert);
				
				mEnemyDamage = -mEnemyDamage;
			}
			else if (newLine == ENEMY_RADIUS_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mEnemyRadius = stof(toConvert);
			}
			else if (newLine == ENEMY_SPAWN_RATE_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mEnemySpawnRate = stof(toConvert);
			}
			else if (newLine == SPEED_BOOST_KEYWORD) //START OF PICKUP DDD
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPickUpSpeedBoost = stof(toConvert);
			}
			else if (newLine == DAMAGE_BOOST_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPickUpDamageBoost = stof(toConvert);
			}
			else if (newLine == ENEMY_HEAL_KEYWORD)
			{
				string toConvert;
				getline(dataFile, toConvert);

				mPickUpHealthBoost = stof(toConvert);
			}
		}

		dataFile.close();
	}
	else
	{
		printf("ERROR! FAILURE TO OPEN FILE!\n");
	}

	//Sets random seed for our random Unit spawns
	srand(time(NULL));
}

UnitManager::~UnitManager()
{
	delete mpSpawnTimer;
	mpSpawnTimer = NULL;

	while (mUnitMap.size() > 0)
	{
		deleteRecentUnit();
	}

	//Enemy Transitions
	if (mpeToDamageEnemyTransition)
	{
		delete mpeToDamageEnemyTransition;
		delete mpeToDieTransition;
		delete mpeToSeekPickUpTransition;
		delete mpeToWanderTransition;

		//Player Transitions
		delete mppToAIControlTransition;
		delete mppToArriveTransition;
		delete mppToMoveTransition;
	}
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id, const int unitType)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		if (id == PLAYER_UNIT_ID)
		{
			pUnit = new (ptr)Unit(sprite, unitType, mPlayerHealth, mPlayerDamage, mPlayerRadius);//placement new
		}
		else
		{
			pUnit = new (ptr)Unit(sprite, unitType, mEnemyHealth, mEnemyDamage, mEnemyRadius);//placement new
		}

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's

		if (id == PLAYER_UNIT_ID)
		{
			pUnit->mMaxSpeed = MAX_SPEED * mPlayerMovement;
		}
		else
		{
			pUnit->mMaxSpeed = MAX_SPEED;
		}

		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite, const int unitType)
{
	float posX = gpGame->getGraphicsSystem()->getWidth(), posY = gpGame->getGraphicsSystem()->getHeight();

	float width = gpGame->getGraphicsSystem()->getWidth();
	float height = gpGame->getGraphicsSystem()->getHeight();

	int random = rand() % 4;

	if (unitType == 0)
	{
		switch (random)
		{
		case 0: //Top Left
			{
				posX *= 0.25f;
				posY *= 0.25f;
			}
			break;
		case 1: //Top Right
			{
				posX *= 0.75f;
				posY *= 0.25f;
			}
			break;
		case 2: //Bottom Right
			{
				posX *= 0.75f;
				posY *= 0.75f;
			}
			break;
		case 3: //Bottom Left
			{
				posX *= 0.25f;
				posY *= 0.75f;
			}
			break;
		}
	}
	else
	{
		posX = rand() % (int)posX;
		posY = rand() % (int)posY;

		if (posX < mEnemyRadius)
		{
			posX = mEnemyRadius;
		}
		else if (posX > width - mEnemyRadius)
		{
			posX = width - mEnemyRadius;
		}

		if (posY < mEnemyRadius)
		{
			posY = mEnemyRadius;
		}
		else if (posY > height - mEnemyRadius)
		{
			posY = height - mEnemyRadius;
		}


	}

	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX,posY),0), PhysicsData(Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), 0.0f, 0.0f), INVALID_UNIT_ID, unitType);
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(Steering::INVALID_TYPE);
	}
	return pUnit;
}

void UnitManager::initializeStateMachineTransitions()
{
	//Create Enemy Transitions
	mpeToWanderTransition = new StateTransition(TO_WANDER_TRANSITION, 0);
	mpeToDamageEnemyTransition = new StateTransition(TO_DAMAGE_ENEMY_TRANSITION, 1);
	mpeToDieTransition = new StateTransition(TO_DIE_TRANSITION, 2);
	mpeToSeekPickUpTransition = new StateTransition(TO_SEEK_PICKUP_TRANSITION, 3);

	//Create Player Transitions
	mppToMoveTransition = new StateTransition(TO_MOVE_TRANSITION, 0);
	mppToArriveTransition = new StateTransition(TO_ARRIVE_TRANSITION, 1);
	mppToAIControlTransition = new StateTransition(TO_AI_TRANSITION, 2);
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::deleteRecentUnit()
{
	if (msNextUnitID > 0)
	{
		msNextUnitID--;
	}


	deleteUnit(msNextUnitID);
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::setInitialEnemyUnitState(Unit* pUnit)
{
	StateMachine* tempMachine = pUnit->InitializeStateMachine();

	/////////////////////////////////////
	//	0: Wander State				   //
	//	1: Damage Enemy State		   //
	//	2: Death State				   //
	//	3: Seek Pickup State		   //
	/////////////////////////////////////

	//Create Enemy States

	StateMachineState* pWanderState = new WanderState(0, pUnit, mEnemyRadius);
	StateMachineState* pDamageEnemyState = new DamageEnemyState(1, pUnit, mEnemyRadius);
	StateMachineState* pDeathState = new DeathState(2, pUnit);
	StateMachineState* pSeekPickUpState = new SeekPickupState(3, pUnit, mPickUpSpeedBoost, mPickUpDamageBoost, mPickUpHealthBoost);

	//Adds transitions to the states

	//WanderState Transitions
	pWanderState->addTransition(mpeToDamageEnemyTransition);
	pWanderState->addTransition(mpeToDieTransition);
	pWanderState->addTransition(mpeToSeekPickUpTransition);

	//DamageEnemyState Transitions
	pDamageEnemyState->addTransition(mpeToWanderTransition);
	pDamageEnemyState->addTransition(mpeToDieTransition);

	//SeekPickUpState Transitions
	pSeekPickUpState->addTransition(mpeToWanderTransition);

	//Add states to the state machine
	tempMachine->addState(pWanderState);
	tempMachine->addState(pDamageEnemyState);
	tempMachine->addState(pDeathState);
	tempMachine->addState(pSeekPickUpState);

	//Initial state is WanderState
	tempMachine->setInitialStateID(0);
}

void UnitManager::setIntitalPlayerUnitState()
{
	StateMachine* tempMachine = getPlayerUnit()->InitializeStateMachine();

	/////////////////////////////////////
	//	0: MovingTo State			   //
	//	1: ArrivedAt State	     	   //
	//	2: AI Control State			   //
	/////////////////////////////////////

	mppMovingToState = new MovingToState(0, mPlayerRadius); //'member pointer player'
	mppArrivedAtState = new ArrivedAtState(1, mPlayerRadius);
	mppAIControlState = new AIControlState(2, mPlayerRadius);

	//Add Transitions to the states

	//MovingToState Transitions
	mppMovingToState->addTransition(mppToArriveTransition);
	mppMovingToState->addTransition(mppToAIControlTransition);

	//ArrivedAtState Transitions
	mppArrivedAtState->addTransition(mppToMoveTransition);
	mppArrivedAtState->addTransition(mppToAIControlTransition);

	//AI Control Transitions
	mppAIControlState->addTransition(mppToArriveTransition);

	//Add States to the state machine
	tempMachine->addState(mppMovingToState);
	tempMachine->addState(mppArrivedAtState);
	tempMachine->addState(mppAIControlState);

	//intial state is arrivedAtState
	tempMachine->setInitialStateID(1);
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update();

		if (it->second->isDirty())
		{
			mListToDelete.emplace_back(it->first);
		}
	}

	for (std::list<UnitID>::iterator it = mListToDelete.begin(); it != mListToDelete.end(); ++it)
	{
		deleteUnit(*it);
	}

	mListToDelete.clear();

	if ((mpSpawnTimer->getElapsedTime() / 1000.0) >= (mEnemySpawnRate))
	{
		Unit* pRandomUnit = createRandomUnit(*gpGame->getSpriteManager()->getSprite(5));

		setInitialEnemyUnitState(pRandomUnit);

		//Resets the timer
		mpSpawnTimer->start();
	}
}

void UnitManager::updatePickUps(float elapsedTime)
{
	if (mUnitMap.size() == 0)
	{
		if ((mpSpawnTimer->getElapsedTime() / 1000.0) >= (mEnemySpawnRate * PICKUP_SPAWN_OFFSET))
		{
			int pickUpType = rand() % 3 + 1;

			Unit* pRandomUnit = createRandomUnit(*gpGame->getSpriteManager()->getSprite(pickUpType + 7), pickUpType);

			//Resets the timer
			mpSpawnTimer->start();
		}
	}
}