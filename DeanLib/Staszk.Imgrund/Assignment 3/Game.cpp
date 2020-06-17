#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "InputSystem.h"
#include <fstream>

Game* gpGame = NULL;

const int WIDTH = 512;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

const int LOCAL_BOID_TOTAL = 25;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpReckoningManager(NULL)
	,mpObstacleManager(NULL)
	,mpInputSystem(NULL)
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

	mIsNetworked = setUpNetwork();

	mpInputSystem = new InputSystem;

	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();


	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);
	mpReckoningManager = new UnitManager(MAX_UNITS);
	mpObstacleManager = new UnitManager(MAX_UNITS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");
	mpGraphicsBufferManager->loadBuffer(mObstacleID, "obstacle.png");

	//load Font
	mpFont = new Font("cour.ttf", 18);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	GraphicsBuffer* pObstacleBuffer = mpGraphicsBufferManager->getBuffer(mObstacleID);
	if (pObstacleBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(OBSTACLE_SPRITE_ID, pObstacleBuffer, 0, 0, (float)pObstacleBuffer->getWidth(), (float)pObstacleBuffer->getHeight());
	}

	//setup units

	//KEEP FOR NOW FOR TESTING. (WANDERANDCHASE REQUIRES A PLAYER UNIT TO WORK. WHICH IS CURRENTLY THE DEFAULT IN UNITADDITIONMESSAGE)
	Unit* pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite, false, PositionData(Vector2D(200, -100), 0.0f));
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

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;
	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;

	//Delete messaging system
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpInputSystem;
	mpInputSystem = NULL;

	//Delete unit managers
	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpReckoningManager;
	mpReckoningManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;

	delete mpObstacleManager;
	mpObstacleManager = NULL;

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

	if (mIsNetworked)
	{
		handleNetworking();
	}

	if (mpNetworkInfo->deadReckonThisFrame && !mpNetworkInfo->hasInterpolated)
	{
		handleReckoning(TARGET_ELAPSED_MS);
	}

	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);

	if ((mpNetworkInfo->packetRecieved || mpNetworkInfo->deadReckonThisFrame) || !mIsNetworked)
	{
		//draw background
		Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
		GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
		mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

		//draw units
		mpUnitManager->drawAll();

		if (mpNetworkInfo->deadReckonThisFrame)
		{
			mpReckoningManager->drawAll();
		}

		mpObstacleManager->drawAll();

		//Flips to screen
		mpGraphicsSystem->swap();
	}

	//Process The messages from the messenger system
	mpMessageManager->processMessagesForThisframe();

}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );

	mpNetworkInfo->packetRecieved = 0;
	//mpNetworkInfo->deadReckonThisFrame = 0;

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

void Game::saveWeights()
{
	std::ofstream saveFile("weights.txt");

	if (saveFile.is_open())
	{
		saveFile << cohesionWeight << std::endl;
		saveFile << separationWeight << std::endl;
		saveFile << velocityMatchWeight << std::endl;
		saveFile.close();
	}
}

void Game::loadWeights()
{
	std::ifstream loadFile("weights.txt");
	std::string temp;

	if (loadFile.is_open())
	{
		getline(loadFile, temp);

		cohesionWeight = stof(temp);

		getline(loadFile, temp);

		separationWeight = stof(temp);

		getline(loadFile, temp);

		velocityMatchWeight = stof(temp);

		loadFile.close();
	}
	else
	{
		std::cout << "Failure";
	}
}

void Game::SpawnBoids()
{
	switch (mpNetworkInfo->model)
	{
		case DATA_SHARING:
		{
			Unit* pUnit;

			mpUnitManager->getMap().clear();

			for (int i = 0; i < LOCAL_BOID_TOTAL; i++)
			{
				pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(2), true); // LOCAL : RED
				pUnit->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
			}
		}
		break;

		case DATA_COUPLING:
		{
			Unit* pUnit;

			mpUnitManager->getMap().clear();

			for (int i = 0; i < LOCAL_BOID_TOTAL; i++)
			{
				pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(2), true); // LOCAL : RED
				pUnit->setSteering(Steering::FLOCKING, Vector2D(200, -100), 0);
			}
		}
		break;
	}
}

bool Game::setUpNetwork()
{
	// buffer
	const unsigned int bufferSz = 512;
	char str[bufferSz];

	while (true)
	{
		printf("(L)ocal or (N)etworked?\n");
		fgets(str, bufferSz, stdin);

	
		if (str[0] == 'L' || str[0] == 'l')
		{
			gpGame->getNetworkInfo()->startGame = 1;
			gpGame->getNetworkInfo()->model = DATA_SHARING;
			return false;
		}
		else if(str[0] == 'N' || str[0] == 'n')
		{
			joinGame();
			return true;
		}
		else
		{
			printf("Invalid option\n");
		}
	}
}

void Game::handleReckoning(float elapsedTime)
{
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	Unit* pUnit;

	for (int i = 0; i < mpReckoningManager->getMapSize(); i++)
	{
		pUnit = mpReckoningManager->getUnit(i);

		if (!pUnit)
		{
			continue;
		}

		position = pUnit->getPositionComponent()->getPosition();
		velocity = pUnit->getPhysicsComponent()->getVelocity();
		acceleration = pUnit->getPhysicsComponent()->getAcceleration();

		Vector2D newPos = position + (velocity * elapsedTime) + ((acceleration * 0.5f) * (elapsedTime * elapsedTime));

		pUnit->getPositionComponent()->setPosition(newPos);
	}
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