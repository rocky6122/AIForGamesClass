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

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mIsAIControlled(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpInputSystem(NULL)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	mpInputSystem = new InputSystem;

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
	mpUnitManager->initializeStateMachineTransitions();

	mpPickupManager = new UnitManager(1);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");

	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//Player art
	mpGraphicsBufferManager->loadBuffer(mMoveToColorID, "arrow.png");
	mpGraphicsBufferManager->loadBuffer(mArriveColorID, "greenArrow.png");
	mpGraphicsBufferManager->loadBuffer(mAIControlColorID, "blueArrow.png");

	//Enemy art
	mpGraphicsBufferManager->loadBuffer(mWanderColorID, "enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mDamageColorID, "orangeArrow.png");
	mpGraphicsBufferManager->loadBuffer(mSeekPickUpColorID, "yellowArrow.png");

	// Pickup art
	mpGraphicsBufferManager->loadBuffer(mPickUpSpeedID, "speedPickup.png");
	mpGraphicsBufferManager->loadBuffer(mPickUpHealthID, "healthPickup.png");
	mpGraphicsBufferManager->loadBuffer(mPickUpDamageID, "damagePickup.png");


	//load Font
	mpFont = new Font("cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	//Player Sprites
	GraphicsBuffer* pMoveToBuffer = mpGraphicsBufferManager->getBuffer( mMoveToColorID );
	Sprite* pMoveToSprite = NULL;
	if( pMoveToBuffer != NULL )
	{
		pMoveToSprite = mpSpriteManager->createAndManageSprite( PLAYER_MOVE_TO_SPRITE_ID, pMoveToBuffer, 0, 0, (float)pMoveToBuffer->getWidth(), (float)pMoveToBuffer->getHeight() );
	}
	GraphicsBuffer* pArrivedAtBuffer = mpGraphicsBufferManager->getBuffer(mArriveColorID);
	Sprite* pArriveSprite = NULL;
	if (pArrivedAtBuffer != NULL)
	{
		pArriveSprite = mpSpriteManager->createAndManageSprite(PLAYER_ARRIVE_SPRITE_ID, pArrivedAtBuffer, 0, 0, (float)pArrivedAtBuffer->getWidth(), (float)pArrivedAtBuffer->getHeight());
	}
	GraphicsBuffer* pAIControlBuffer = mpGraphicsBufferManager->getBuffer(mAIControlColorID);
	Sprite* pAIControlSprite = NULL;
	if (pAIControlBuffer != NULL)
	{
		pAIControlSprite = mpSpriteManager->createAndManageSprite(PLAYER_AI_CONTROL_SPRITE_ID, pAIControlBuffer, 0, 0, (float)pAIControlBuffer->getWidth(), (float)pAIControlBuffer->getHeight());
	}

	//Enemy Sprites
	GraphicsBuffer* pWanderBuffer = mpGraphicsBufferManager->getBuffer(mWanderColorID);
	Sprite* pWanderSprite = NULL;
	if (pWanderBuffer != NULL)
	{
		pWanderSprite = mpSpriteManager->createAndManageSprite(AI_WANDER_SPRITE_ID, pWanderBuffer, 0, 0, (float)pWanderBuffer->getWidth(), (float)pWanderBuffer->getHeight());
	}
	GraphicsBuffer* pDamageBuffer = mpGraphicsBufferManager->getBuffer(mDamageColorID);
	Sprite* pDamageSprite = NULL;
	if (pDamageBuffer != NULL)
	{
		pDamageSprite = mpSpriteManager->createAndManageSprite(AI_DAMAGE_SPRITE_ID, pDamageBuffer, 0, 0, (float)pDamageBuffer->getWidth(), (float)pDamageBuffer->getHeight());
	}
	GraphicsBuffer* pSeekPickUpBuffer = mpGraphicsBufferManager->getBuffer(mSeekPickUpColorID);
	Sprite* pSeekPickUpSprite = NULL;
	if (pSeekPickUpBuffer != NULL)
	{
		pSeekPickUpSprite = mpSpriteManager->createAndManageSprite(AI_SEEK_PICKUP_SPRITE_ID, pSeekPickUpBuffer, 0, 0, (float)pSeekPickUpBuffer->getWidth(), (float)pSeekPickUpBuffer->getHeight());
	}

	// Pickup sprites
	GraphicsBuffer* pPickupSpeedBuffer = mpGraphicsBufferManager->getBuffer(mPickUpSpeedID);
	Sprite* pPickupSpeedSprite = NULL;
	if (pPickupSpeedBuffer != NULL)
	{
		pPickupSpeedSprite = mpSpriteManager->createAndManageSprite(PICKUP_SPEED, pPickupSpeedBuffer, 0, 0, (float)pPickupSpeedBuffer->getWidth(), (float)pPickupSpeedBuffer->getHeight());
	}
	GraphicsBuffer* pPickupHealthBuffer = mpGraphicsBufferManager->getBuffer(mPickUpHealthID);
	Sprite* pPickupHealthSprite = NULL;
	if (pPickupHealthBuffer != NULL)
	{
		pPickupHealthSprite = mpSpriteManager->createAndManageSprite(PICKUP_HEALTH, pPickupHealthBuffer, 0, 0, (float)pPickupHealthBuffer->getWidth(), (float)pPickupHealthBuffer->getHeight());
	}
	GraphicsBuffer* pPickupDamageBuffer = mpGraphicsBufferManager->getBuffer(mPickUpDamageID);
	Sprite* pPickupDamageSprite = NULL;
	if (pWanderBuffer != NULL)
	{
		pPickupDamageSprite = mpSpriteManager->createAndManageSprite(PICKUP_DAMAGE, pPickupDamageBuffer, 0, 0, (float)pPickupDamageBuffer->getWidth(), (float)pPickupDamageBuffer->getHeight());
	}
	

	//setup units
	Unit* pUnit = mpUnitManager->createPlayerUnit(*pArriveSprite);
	pUnit->setShowTarget(false);
	pUnit->setSteering(Steering::ARRIVEANDFACE, ZERO_VECTOR2D);
	mpUnitManager->setIntitalPlayerUnitState();

	/*Unit* pPickup = mpPickupManager->createRandomUnit(*mpSpriteManager->getSprite(8), 1);
	pPickup->setShowTarget(false);
	pPickup->setSteering(Steering::INVALID_TYPE);*/

	mpMasterTimer->start();

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

	delete mpInputSystem;
	mpInputSystem = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpPickupManager;
	mpPickupManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpInputSystem->processInputs();

	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpPickupManager->updatePickUps(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();
	mpPickupManager->drawAll();

	//SDL_PumpEvents();
	int x, y;
	SDL_GetMouseState(&x, &y);

	//create mouse text
	std::stringstream mousePos;
	mousePos << x << ":" << y;

	//write text at mouse position
	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y, mousePos.str(), BLACK_COLOR);

	//Flip to screen
	mpGraphicsSystem->swap();

	//Process The messages from the messenger system
	mpMessageManager->processMessagesForThisframe();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );

	if (mpUnitManager->getPlayerUnit()->isDead() && mShouldExit) //Displays the final game info before closing.
	{
		//draw background
		Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
		GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
		mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

		std::stringstream scoreDisplay;
		scoreDisplay << "You survived for: " << (float)(mpMasterTimer->getElapsedTime() / 1000.0) << " seconds!";

		mpGraphicsSystem->writeText(*mpFont, (float)WIDTH / 3, (float)HEIGHT / 2 - 100, "Game Over!", BLACK_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)WIDTH / 2 - 200, (float)HEIGHT / 2, scoreDisplay.str(), BLACK_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)WIDTH / 3, (float)HEIGHT / 2 + 100, "Press ENTER key to continue...", BLACK_COLOR);

		mpGraphicsSystem->swap();

		while (true)
		{
			if (mpInputSystem->waitingForInput())
			{
				break;
			}
		}
	}

	mpMasterTimer->stop();

	return mShouldExit;
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