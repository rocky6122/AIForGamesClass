#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>

#include "Networking.h"

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
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType OBSTACLE_SPRITE_ID = 4;

const float LOOP_TARGET_TIME = 33.33f;//how long should each frame of execution take? 30fps = 33.33ms/frame

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
	inline UnitManager* getReckoningManager() { return mpReckoningManager; };
	inline UnitManager* getObstacleManager() { return mpObstacleManager; };

	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline NetworkInfo* getNetworkInfo() const { return mpNetworkInfo; };
	
	//Had to make this public so the input system could interact with it
	//If a better way is found to do this please execute it and let me know - John Imgrund
	bool mShouldExit;

	float cohesionWeight = 2.0f;
	float separationWeight = 1.0f;
	float velocityMatchWeight = 1.0f;

	// Updates the weights for the flocking boids
	void updateFlock();

	//Save Flocking Weights to file
	void saveWeights();
	// Load flocking weights from file
	void loadWeights();

	// Spawns the boids
	void SpawnBoids();

private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	UnitManager* mpReckoningManager;
	UnitManager* mpObstacleManager;
	Font* mpFont;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	InputSystem* mpInputSystem; //Added to place an input system into the game - John Imgrund
	NetworkInfo* mpNetworkInfo;
	
	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mObstacleID = "obstacle";

	// functions
	bool setUpNetwork();

	void handleReckoning(float elapsedTime);

	bool mIsNetworked = false;
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;