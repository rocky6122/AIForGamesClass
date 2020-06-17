#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class Font;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class InputSystem;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType TARGET_SPRITE_ID = 1;
const IDType PLAYER_MOVE_TO_SPRITE_ID = 2;
const IDType PLAYER_ARRIVE_SPRITE_ID = 3;
const IDType PLAYER_AI_CONTROL_SPRITE_ID = 4;
const IDType AI_WANDER_SPRITE_ID = 5;
const IDType AI_DAMAGE_SPRITE_ID = 6;
const IDType AI_SEEK_PICKUP_SPRITE_ID = 7;
const IDType PICKUP_SPEED = 8;
const IDType PICKUP_HEALTH = 9;
const IDType PICKUP_DAMAGE = 10;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game:public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline UnitManager* getPickupManager() { return mpPickupManager; };
	inline std::string getDataDrivenFile() const { return DATA_DRIVEN_FILE; };
	
	//Had to make this public so the input system could interact with it
	//If a better way is found to do this please execute it and let me know - John Imgrund
	bool mShouldExit;
	bool mIsAIControlled;

private:

	//Official name of the TextFile for the Data Driven Values
	const std::string DATA_DRIVEN_FILE = "readFile.txt";

	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;

	UnitManager* mpUnitManager;
	UnitManager* mpPickupManager;

	Font* mpFont;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	InputSystem* mpInputSystem; //Added to place an input system into the game - John Imgrund

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mMoveToColorID = "moveTo";
	GraphicsBufferID mArriveColorID = "arrive";
	GraphicsBufferID mAIControlColorID = "AIControl";
	GraphicsBufferID mWanderColorID = "wander";
	GraphicsBufferID mDamageColorID = "damage";
	GraphicsBufferID mSeekPickUpColorID = "seekPickUp";
	GraphicsBufferID mPickUpSpeedID = "pickupSpeed";
	GraphicsBufferID mPickUpHealthID = "pickupHealth";
	GraphicsBufferID mPickUpDamageID = "pickupDamage";
	
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;