#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>
#include "Game.h"
#include "Timer.h"

#include "GameMessageManager.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "InputSystem.h"
#include <fstream>

Game* gpGame = NULL;



const Uint32 MAX_UNITS = 100;

const int LOCAL_BOID_TOTAL = 25;

Game::Game()
	:mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mShouldExit(false)
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpObstacleManager(NULL)
	,mpInputSystem()
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

	mpNetworkInfo = new NetworkInfo;
	mpNetworkInfo->peer = peer;

	mpNetworkInfo->myAddress = mpNetworkInfo->peer->GetSystemAddressFromGuid(mpNetworkInfo->peer->GetMyGUID());

	setUpNetwork();

	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);
	mpObstacleManager = new UnitManager(MAX_UNITS);

	//setup units

	//KEEP FOR NOW FOR TESTING. (WANDERANDCHASE REQUIRES A PLAYER UNIT TO WORK. WHICH IS CURRENTLY THE DEFAULT IN UNITADDITIONMESSAGE)
	Unit* pUnit = mpUnitManager->createPlayerUnit(false, PositionData(Vector2D(200, -100), 0.0f));
	pUnit->setShowTarget(true);
	pUnit->setSteering(Steering::ARRIVEANDFACE, Vector2D(200,-200));

	//KEEP AS EXAMPLE OF HOW TO CREATE AN EXTRA 'BOID' JUST IN CASE. DELETE AT END OF ASSIGNMENT 3 - JOHN IMGRUND

	//pUnit = mpUnitManager->createUnit(*pEnemyArrow, true, PositionData(Vector2D((float)gpGame->getGraphicsSystem()->getWidth()-1, 0.0f), 0.0f));
	//pUnit->setShowTarget(false);
	//pUnit->setSteering(Steering::WANDERANDCHASE, ZERO_VECTOR2D);

	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpObstacleManager;
	mpObstacleManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;

	//RakNet::RakPeerInterface::DestroyInstance(mpNetworkInfo->peer);
	delete mpNetworkInfo;
	mpNetworkInfo = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	if (mpNetworkInfo->startGame)
	{
		SpawnBoids();

		mpNetworkInfo->isRunning = 1;
		mpNetworkInfo->startGame = 0;
	}


	// Get Input
	mpInputSystem->processInputs();

	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);

	handleNetworking();

	//Process The messages from the messenger system
	mpMessageManager->processMessagesForThisframe();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

void Game::updateFlock()
{
	for (int i = 1; i < getUnitManager()->getMapSize(); i++)
	{
		Unit* temp = getUnitManager()->getUnit(i);

		temp->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
	}
}

void Game::SpawnBoids()
{
	switch (mpNetworkInfo->model)
	{
		case DATA_PUSH:
		{
			Unit* pUnit;
			
			mpUnitManager->getMap().clear();

			for (int i = 0; i < TOTAL_BOID_NUM; i++)
			{
				pUnit = mpUnitManager->createRandomUnit(true); // LOCAL : RED
				pUnit->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
			}
		}
		break;

		case DATA_SHARING:
		{
			//Unit* pUnit;

			//for (int i = 0; i < LOCAL_BOID_TOTAL; i++)
			//{
			//	pUnit = mpUnitManager->createRandomUnit(true); // LOCAL : RED
			//	pUnit->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
			//}
		}
		break;

		case DATA_COUPLING:
		{
			//Unit* pUnit;

			//for (int i = 0; i < LOCAL_BOID_TOTAL; i++)
			//{
			//	pUnit = mpUnitManager->createRandomUnit(true); // LOCAL : RED
			//	pUnit->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
			//}
		}
		break;
	}
}

void Game::setUpNetwork()
{
	hostGame();
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}